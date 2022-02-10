

#include <pthread.h>
#include <memory>
#include <stack>
namespace rtos{
    template<typename T>
    class Task{
        public:
            Task(std::stack<T>& _stack) : m_input_stack{_stack}{}
            virtual void run() = 0;
            virtual ~Task(){}
        protected:
            std::stack<T>& m_input_stack;
    };

    template<typename T>
    class Thread{
        public:
            Thread(std::shared_ptr<Task<T>> run, bool isDetached = false) : m_runnable{run}, detached{isDetached}{
                if(!m_runnable.get()){
                    throw "Runnable object is null";
                }
            }

            Thread(Task<T>* run, bool isDetached) : detached{isDetached}{
                m_runnable.reset(run);
                if(!m_runnable.get()){
                    throw "Runnable object is null";
                }
            }

            Thread(std::shared_ptr<Task<T>> run, bool isDetached, int policy, int priority) : detached{isDetached}, m_runnable{run}{
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

            void join(){
                if(!detached){
                    int status = pthread_join(this->m_thread_id, nullptr);
                    if(status) throw "Error when joining thread";
                }
            }
            pthread_t get_thread_id(){
                return this->m_thread_id;
            }
            bool is_completed(){
                return this->completed;
            }
            // T getResult(){
            //     return (this->m_result);
            // }

        private:
            pthread_t m_thread_id;
            bool detached;
            bool completed;
            pthread_attr_t m_thread_attr;
            struct sched_param m_thread_params;
            std::shared_ptr<Task<T>> m_runnable;
            static void* startThread(void* arg){
                Thread* this_thread = static_cast<Thread*>(arg);
                if(this_thread == nullptr) throw "Invalid task provided";
                this_thread->m_runnable->run();
                return nullptr;
            }
    };
}