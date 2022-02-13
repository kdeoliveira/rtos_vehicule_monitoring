// #pragma once


// namespace rtos{

//     template<typename T>
//     class buffer{
//         public:
//             buffer(int _sz) : m_sz{_sz}{
//                 if(this->m_sz < 0) throw "Out of range";
//                 m_buf = new T[this->m_sz];
//             }

//             ~buffer(){
//                 delete m_buf;
//             }
            

//             T at(const int i) const{
//                 if(i < 0 || i >= this->m_sz) throw "Out of range";

//                 return this->m_buf[i];
//             }

//             void add(const T& item, const int i){
//                 if(i < 0 || i >= this->m_sz) throw "Out of range";
//                 this->m_buf[i] = item;
//             }

//             T& pop(const int i){
//                 if(i < 0 || i >= this->m_sz) throw "Out of range";
//                 T* out = &this->m_buf[i];
//                 this->m_buf[i] = nullptr;
//                 return *out;
//             }

//             constexpr int size() const{
//                 return this->m_sz;
//             }


//             T& operator[](int i){
//                 if(i < 0 || i >= this->m_sz) throw "Out of range";
//                 return this->m_buf[i];
//             }

//             T* begin(){
//                 return this->m_sz ? &this->m_buf[0] : nullptr;
//             }

//             T* end(){
//                 return (&this->m_buf[0]) + this->m_sz;
//             }




//         private:
//             int m_sz;
//             T* m_buf;
//     };
// }







#pragma once
#include <rtos_thread_safe.hpp>

namespace rtos{

    template<typename T>
    class buffer : public thread_safe<T>{
        public:
            buffer(int _sz) : m_sz{_sz}{
                if(this->m_sz < 0) throw "Out of range";
                m_buf = new T[this->m_sz];
            }

            ~buffer(){
                delete m_buf;
            }

            T* begin(){
                return this->m_sz ? &this->m_buf[0] : nullptr;
            }

            T* end(){
                return (&this->m_buf[0]) + this->m_sz;
            }

            constexpr int size() const{
                return this->m_sz;
            }

            
            T& at(const int i){
                if(i < 0 || i >= this->m_sz) throw "Out of range";

                return this->m_buf[i];
            }

        protected:


            void __add(const T& item, const int i) override{
                if(i < 0 || i >= this->m_sz) throw "Out of range";
                this->m_buf[i] = item;
            }

            T& __pop(const int i) override{
                if(i < 0 || i >= this->m_sz) throw "Out of range";
                T* out = &this->m_buf[i];
                this->m_buf[i] = T{};
                return *out;
            }

            T& __access(int i) override{
                if(i < 0 || i >= this->m_sz) throw "Out of range";
                return this->m_buf[i];
            }

            // T& operator[](int i){
            //     if(i < 0 || i >= this->m_sz) throw "Out of range";
            //     return this->m_buf[i];
            // }

            

            




        private:
            int m_sz;
            T* m_buf;
    };
}