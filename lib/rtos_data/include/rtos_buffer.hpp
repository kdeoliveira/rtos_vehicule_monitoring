#pragma once
#include <rtos_thread_safe.hpp>
#include <utility>

namespace rtos{

    /**
     * @brief Thread safe buffer STL container using traditional raw arrays
     * 
     * @tparam T type of this container
     */
    template<typename T>
    class buffer : public thread_safe<T>{
        public:
            buffer() : m_buf{new T{}} {}

            /**
             * @brief Creates a new buffer of size _sz
             * 
             * @param _sz Size of array
             */
            buffer(int _sz) : m_sz{_sz}{
                if(this->m_sz < 0) throw "Out of range";
                m_buf = new T[this->m_sz];
            }

            
            buffer(const buffer& in) : m_sz{in.m_sz}{ 
                std::copy(in.m_buf, in.m_buf + this->m_sz, this->m_buf);
            }

            buffer(buffer&& in) noexcept : buffer(){ 
                swap(*this, in);
            }

            buffer& operator=(buffer& in){
                swap(*this, in);
                return *this;
            }

            template<typename R>
            buffer& operator=(buffer<R>&& in){
                swap(*this, in );
                return *this;
            }

            


            /// @private
            ~buffer(){
                delete[] m_buf;
            }

            /// @private
            T* begin(){
                return this->m_sz ? &this->m_buf[0] : nullptr;
            }

            /// @private
            T* end(){
                return (&this->m_buf[0]) + this->m_sz;
            }

            /**
             * @brief Returns the current size of this container
             * 
             * @return constexpr int 
             */
            constexpr int size() const{
                return this->m_sz;
            }

            /// @private
            T& at(const int i){
                if(i < 0 || i >= this->m_sz) throw "Out of range";

                return this->m_buf[i];
            }

        private:
            friend void swap(buffer& first, buffer& second){
                using std::swap;
                swap(first.m_buf, second.m_buf);
                swap(first.m_sz, second.m_sz);
            }

        protected:

            /// @private
            void __add(const T& item, const int i) override{
                if(i < 0 || i >= this->m_sz) throw "Out of range";
                this->m_buf[i] = item;
            }

            /// @private
            T& __pop(const int i) override{
                if(i < 0 || i >= this->m_sz) throw "Out of range";
                T* out = &this->m_buf[i];
                this->m_buf[i] = T{};
                return *out;
            }

            /// @private
            T& __access(int i) override{
                if(i < 0 || i >= this->m_sz) throw "Out of range";
                return this->m_buf[i];
            }

        private:
            int m_sz;
            T* m_buf;
    };
}