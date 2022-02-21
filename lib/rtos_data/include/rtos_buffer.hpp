#pragma once
#include <rtos_thread_safe.hpp>
#include <utility>

namespace rtos{

    template<typename T>
    class buffer : public thread_safe<T>{
        public:
            buffer() : m_buf{new T{}} {}

            buffer(int _sz) : m_sz{_sz}{
                if(this->m_sz < 0) throw "Out of range";
                m_buf = new T[this->m_sz];
            }


            buffer(const buffer& in) : m_sz{in.m_sz}{ //copy constructor
                std::copy(in.m_buf, in.m_buf + this->m_sz, this->m_buf);
            }

            buffer(buffer&& in) noexcept : buffer(){ //move constructor
                swap(*this, in);
            }

            buffer& operator=(buffer& in){
                swap(*this, in);
                return *this;
            }

            template<typename R>
            buffer& operator=(buffer<R>&& in){
                swap(*this, std::forward<typename std::remove_reference<R>::type>(in) );
                return *this;
            }

            



            ~buffer(){
                delete[] m_buf;
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

        private:
            friend void swap(buffer& first, buffer& second){
                //ADT for std lib swap
                using std::swap;
                swap(first.m_buf, second.m_buf);
                swap(first.m_sz, second.m_sz);
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