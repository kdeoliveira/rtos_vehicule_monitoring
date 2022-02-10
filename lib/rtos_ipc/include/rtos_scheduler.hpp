#pragma once
#include <queue>
#include <signal.h>
#include <memory>
#include "rtos_timer.hpp"
#include "rtos_algorithm.hpp"
#include "rtos_ipc.hpp"

namespace rtos{
    
    template<typename T>
    class Scheduler{
        public:
            Scheduler() = delete;

            Scheduler(int signmum, std::shared_ptr<algorithm<T>> _algorithm) : m_signum{signmum}{
                if(m_algorithm == nullptr) throw "Algorithm object is null";
                this->m_algorithm = _algorithm;
                this->m_cycles(5);
            }

            Scheduler(int signmum, algorithm<T>* _algorithm) : m_signum{signmum}, m_cycles(5){
                // if(m_algorithm == nullptr) throw "Algorithm object is null";
                this->m_algorithm.reset(_algorithm);
                
            }

            Scheduler(const Scheduler&) = delete;

            ~Scheduler(){
                // delete this->m_algorithm;
                // delete m_cycles;
                
            }

            // void push(period_task arg){
            //     if(this->m_cycles.cycles == this->m_sz - 1){
            //         return;
            //     }
            //     m_cycles++;
            //     this->m_queue[m_cycles.cycles] = arg;
            // }

            // period_task pop(){
            //     if(this->m_cycles == 0)  return;
            //     period_task task = this->m_queue[this->m_cycles.cycles];
            //     this->m_cycles++;
            //     return task;
            // }



            void dispatch(){
                
                siginfo_t info;
                sigset_t set = util::mask_signal(this->m_signum);
                
                while(true){
                    if(this->m_cycles.cycles < 0) return;

                    
                    
                    sigwaitinfo(&set, &info);
                    
                    if(this->m_algorithm != nullptr){
                        
                        this->m_algorithm->run(&this->m_cycles, this->m_signum);
                    }
                    
                }
            }



        private:
            //Implement thread safe queue
            std::shared_ptr<algorithm<T>> m_algorithm;
            timer_cycle m_cycles;
            const int m_signum;
            std::unique_ptr<pid_t> m_pid_id;


    };
}