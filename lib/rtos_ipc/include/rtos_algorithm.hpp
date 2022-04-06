#pragma once

#include "rtos_timer.hpp"


namespace rtos{
    /**
     * @brief Strategy interface called by the Scheduler
     * 
     * @tparam T type of task
     */
    template<typename T>
    class algorithm{
        public:
            algorithm() = delete;
            algorithm(T* que) : m_queue{que}{}
            ~algorithm(){
                delete this->m_queue;
            }

            /**
             * @brief Construct a new algorithm object which can hold up to _sz tasks
             * 
             * @param _sz Number of tasks managed by this algorithm
             */
            algorithm(const int _sz) : sz{_sz}{
                if(_sz < 0) throw "Invalid size argument";

                this->m_queue = new T[sz];
                m_index = 0;
            }
            
            /**
             * @brief Virtual function that is called by the Scheduler
             * Algorithm should be implemented here
             * 
             * @param _timer_cycle current cycle of the timer
             * @param _signmum signal received by the timer
             * @return void* 
             */
            virtual void * run(timer_cycle* _timer_cycle, const int& _signmum) const = 0;

            /**
             * @brief Returns a pointer to the set of tasks maanaged by this algorithm
             * 
             * @return T* pointer to set of tasks
             */
            T* get_queue(){
                return this->m_queue;
            }

            constexpr int size() const{
                return this->sz;
            }



        protected:
            T* m_queue;
            const int sz;
            uint8_t m_index;
            




    };
}