#include <queue>
#include <signal.h>
#include <memory>

namespace rtos{

    
    struct period_task{
        pthread_t thread_id;
        uint8_t period;
    };

    
    class Scheduler{
        public:
            Scheduler(int sz) : m_sz{sz}{
                this->m_queue = new period_task[m_sz];

            }

            ~Scheduler(){
                
            }

            void push(period_task arg){
                if(this->m_index == this->m_sz - 1){
                    return;
                }

                this->m_queue[m_index++] = arg;
            }

            void pop(){
                if(this->m_index == 0)  return;
                this->m_index--;
            }



            int on_kill(){
                if(this->m_index == 0) return -1;

                period_task next = this->m_queue[this->m_index++];

                int status = pthread_kill(next.thread_id, this->m_signum);

                if(status == -1) throw "Error notifying thread";


            }



        private:
            //Implement thread safe queue
            period_task* m_queue;
            const int m_sz;
            int m_index;
            int m_signum;
            std::unique_ptr<pid_t> m_pid_id;


    };
}