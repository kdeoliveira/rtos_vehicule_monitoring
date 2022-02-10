#pragma once

#include "rtos_timer.hpp"


namespace rtos{
    template<typename T>
    class algorithm{
        public:
            algorithm() = delete;
            ~algorithm(){
                delete this->m_queue;
            }

            algorithm(const int _sz) : sz{_sz}{
                if(_sz < 0) throw "Invalid size argument";

                this->m_queue = new T[sz];
                m_index = 0;
            }

            virtual void * run(timer_cycle* _timer_cycle, const int& _signmum) const = 0;

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