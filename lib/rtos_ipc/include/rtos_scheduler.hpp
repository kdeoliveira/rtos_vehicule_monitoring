#pragma once
#include <queue>
#include <signal.h>
#include <memory>
#include "rtos_timer.hpp"
#include "rtos_algorithm.hpp"
#include "rtos_ipc.hpp"

namespace rtos{
    
    /**
     * @brief Context interface that implements a scheduler
     * The scheduler is responsible for "waking-up" threads at any given moment.
     * Note that threads receiving signals from the Scheduler should have the appropriate signal masked
     * @tparam T type of task
     */
    template<typename T>
    class Scheduler{
        public:
            Scheduler() = delete;


            Scheduler(int signmum, std::shared_ptr<algorithm<T>> _algorithm, const u_int8_t number_of_cycles) : m_signum{signmum}{
                if(m_algorithm == nullptr) throw "Algorithm object is null";
                this->m_algorithm = _algorithm;
                //Equiavelent to a Hyperperiod
                this->m_cycles(number_of_cycles);
            }
            /**
             * @brief Construct a new Scheduler object.
             * Note that in order to avoid long integers, the Scheduler will run over a cyclic period [0 , Hyperperoid]
             * 
             * @param signmum Signal used by this scheduler to signal other threads
             * @param _algorithm Algorithm implementation for this scheduler
             * @param number_of_cycles Hyperperiod of this scheduler. 
             */
            Scheduler(int signmum, algorithm<T>* _algorithm, const u_int8_t number_of_cycles) : m_cycles(number_of_cycles), m_signum{signmum}{
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


            /**
             * @brief Function responsible for executing the algorihtm interface at every clock cycle
             * 
             * @param _sig Signal received by this scheduler
             */
            void dispatch(const int _sig){
                
                siginfo_t info;
                // sigset_t set = util::mask_signal(this->m_signum);
                sigset_t set;
                sigaddset(&set, this->m_signum);
                
                while(true){
                    if(this->m_cycles.cycles < 0) return;
                    sigwaitinfo(&set, &info);
                    if(this->m_algorithm != nullptr){

                        this->m_algorithm->run(&this->m_cycles, _sig);
                    }

                    ++this->m_cycles;

                    
                }
            }



        private:
            std::shared_ptr<algorithm<T>> m_algorithm;
            timer_cycle m_cycles;
            const int m_signum;
            std::unique_ptr<pid_t> m_pid_id;


    };
}