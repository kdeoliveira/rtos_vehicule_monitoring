

#include <pthread.h>
#include <memory>
#include <stack>
#include <rtos_ipc.hpp>

namespace rtos{
    /**
     * @brief Runnable object that is executed by a thread
     * 
     * @tparam T 
     */
    template<typename T>
    class Task{
        public:
            virtual void run() = 0;
            virtual ~Task(){}
    };

    /**
     * @brief POSIX Thread class implementation
     * 
     * @tparam T 
     */
    template<typename T>
    class Thread{
        public:
            Thread(std::shared_ptr<Task<T>> run, bool isDetached, const int signum) : m_runnable{run}, detached{isDetached}, m_signum{signum}{
                if(!m_runnable.get()){
                    throw "Runnable object is null";
                }
            }

            /**
             * @brief Construct a new Thread object that will execute a given task
             * 
             * @param run Runnable instance
             * @param isDetached Set thread as detachable
             * @param signum Signal which this thread will be sensitive to
             */
            Thread(Task<T>* run, bool isDetached, const int signum) : detached{isDetached}, m_signum{signum}{
                m_runnable.reset(run);
                if(!m_runnable.get()){
                    throw "Runnable object is null";
                }
            }

            Thread(std::shared_ptr<Task<T>> run, bool isDetached, int policy, int priority, const int signum) : detached{isDetached}, m_runnable{run}, m_signum{signum}{
                pthread_attr_setinheritsched(&this->m_thread_attr, PTHREAD_EXPLICIT_SCHED);

                int status = pthread_attr_setschedpolicy(&this->m_thread_attr, policy);
                if(status) throw "Invalid pthread_attr";

                this->m_thread_params.sched_priority = priority;
                status = pthread_attr_setschedparam(&this->m_thread_attr, &this->m_thread_params);
                if(status) throw "Invalid pthread_attr";
            }

            Thread(const Thread&);
            const Thread& operator=(const Thread&);

            virtual ~Thread(){
                pthread_attr_destroy(&this->m_thread_attr);

            }

            /**
             * @brief Starts the execution of this thread by calling internally Thread::startThread
             * The thread remains suspended until the specified SIGNUM signal becomes pending
             * 
             */
            void start(){
                int status = pthread_attr_init(&this->m_thread_attr);
                if(status) throw "Invalid pthread_attr";

                status = pthread_attr_setdetachstate(&m_thread_attr, detached ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE);
                if(status) throw "Invalid pthread_attr";
                status = pthread_create(&this->m_thread_id, &this->m_thread_attr, Thread::startThread, (void*) this);
                
                if(status){
                    throw "Error pthread_create";
                }else{
                    this->completed = true;
                }
                
            }

            /**
             * @brief Waits for termination of this thread
             * 
             */
            void join(){
                if(!detached){
                    int status = pthread_join(this->m_thread_id, nullptr);
                    if(status) throw "Error when joining thread";
                }
            }

            /**
             * @brief Get the thread id
             * 
             * @return pthread_t 
             */
            pthread_t get_thread_id(){
                return this->m_thread_id;
            }

            /**
             * @brief Checks if the runnable task has finished execution
             * 
             * @return bool 
             */
            bool is_completed(){
                return this->completed;
            }

        private:
            pthread_t m_thread_id;
            bool detached;
            bool completed;
            pthread_attr_t m_thread_attr;
            struct sched_param m_thread_params;
            std::shared_ptr<Task<T>> m_runnable;
            const int m_signum;

            static void* startThread(void* arg){
                Thread* this_thread = static_cast<Thread*>(arg);
                if(this_thread == nullptr) throw "Invalid task provided";
                sigset_t set = util::mask_signal(this_thread->m_signum);
                
                pthread_sigmask(this_thread->m_signum, &set, nullptr);
                siginfo_t info;

                while(true){
                    sigwaitinfo(&set, &info);
                    this_thread->m_runnable->run();
                }
                return nullptr;
            }
    };
}