#pragma once
#include <queue>
#include <signal.h>
#include <memory>
#include <rtos_timer.hpp>



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
                delete this->m_queue;
            }

            void push(period_task arg){
                if(this->m_cycles.cycles == this->m_sz - 1){
                    return;
                }
                m_cycles++;
                this->m_queue[m_cycles.cycles] = arg;
            }

            period_task pop(){
                if(this->m_cycles == 0)  return;
                period_task task = this->m_queue[this->m_cycles.cycles];
                this->m_cycles++;
                return task;
            }



            int on_kill(){

                siginfo_t info;
                sigset_t set;
                sigemptyset(&set);
                sigaddset(&set, this->m_signum);
                sigprocmask(SIG_BLOCK, &set,nullptr);
                
                while(true){
                    if(this->m_cycles < 0) return -1;
                    period_task next = this->pop();

                    sigwaitinfo(&set, &info);
                    
                    int status = pthread_kill(next.thread_id, this->m_signum);
                    if(status == -1) throw "Error notifying thread";
                }
            }



        private:
            //Implement thread safe queue
            period_task* m_queue;
            const int m_sz;
            timer_cycle m_cycles;
            int m_signum;
            std::unique_ptr<pid_t> m_pid_id;


    };
}