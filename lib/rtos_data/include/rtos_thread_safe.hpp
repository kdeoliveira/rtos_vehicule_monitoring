

#pragma once

#include <mutex>
#include <condition_variable>

namespace rtos{

    template<typename T>
    class thread_safe{
        public:
        
            virtual ~thread_safe(){}

            void add(const T& item, const int i){
                std::unique_lock ul(this->m_mx);
                this->__add(item, i);
            }

            T& operator[](int i){
                std::unique_lock ul(this->m_mx);
                return this->__access(i);
            }

            T& pop(const int i){
                std::unique_lock ul(this->m_mx);
                return this->__pop(i);
            }




        protected:
            virtual void __add(const T&, const int) = 0;
            virtual T& __pop(const int) = 0;
            virtual T& __access(int) = 0;



        private:
            std::mutex m_mx;
            //add cv for adding or accessing properties on producer-consumer algo with internal flag
            std::condition_variable m_cv;
    
    };
}