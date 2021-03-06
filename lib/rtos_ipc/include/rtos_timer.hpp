#pragma once
#include "rtos_ipc.hpp"
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <rtos_common.hpp>
#include <vector>
#include <errno.h>

#ifndef _QNX_x86_64
    #include <sys/syscall.h>

    #ifndef SYS_gettid
        #error "SYS_gettid unavailable on this system"
    #endif

    #define gettid() ((pid_t)syscall(SYS_gettid))
#endif

namespace rtos{
    /*!
     * @brief Counter for timer period
     */
    struct timer_cycle{
        u_int8_t hyperperiod = 1;
        u_int8_t cycles = 0;

        timer_cycle(const u_int8_t _hyperperiod) : hyperperiod{_hyperperiod}{
        }

        timer_cycle& operator++(){
            (*this).cycles = ((*this).cycles + 1) % (*this).hyperperiod;
            return *this;
        }

        timer_cycle operator++(int){
            (*this).cycles = ((*this).cycles + 1) % (*this).hyperperiod;
            return *this;
        }

        timer_cycle& operator--(){

            if ((*this).cycles == 0 ) return *this;
            (*this).cycles = ((*this).cycles - 1) % (*this).hyperperiod;
            return *this;
        }


        timer_cycle operator--(int){
            if ((*this).cycles == 0 ) return *this;
            (*this).cycles = ((*this).cycles - 1) % (*this).hyperperiod;
            return *this;
        }

    };

    /**
     * @brief Creates a new POSIX interval timer which upon expiration throws a signal to this process.
     * This same objects then waits for the above signal to be pending before executing a registered callback function
     * 
     */
    class Timer{
        public:
            Timer(const Timer&) = delete;
            Timer() = delete;
            /**
             * @brief Construct a new Timer object
             * 
             * @param clock_type specifies the clock that the new timer uses to measure time
             * @param signum 
             */
            Timer(clockid_t clock_type, int signum){
                
                this->callback = nullptr;

                // if(signum > SIGRTMAX || signum < SIGRTMIN) throw "Signal is out of range";
                m_sigevent = new struct sigevent;
                
                #ifndef _QNX_x86_64
                    this->m_sigevent->sigev_notify = SIGEV_THREAD_ID;
                    
                    this->m_sigevent->_sigev_un._tid = gettid();
                #else
                    this->m_sigevent->sigev_notify = SIGEV_SIGNAL;
                #endif

                this->m_sigevent->sigev_signo = signum;

                
                this->m_timer_id = timer_create(clock_type, this->m_sigevent, &this->m_timer);


                if(m_timer_id < 0) throw "Unable to create timer";
            }


            /**
             * @brief Sets the timer to the given value and starts the timer
             * 
             * @param period_seconds Seconds
             * @param period_nanoseconds Nano seconds
             * @return int Returns 0 if successful; otherwise returns -1
             */
            int start(int period_seconds, int period_nanoseconds){
                m_timerspec.it_value.tv_sec = period_seconds;
                m_timerspec.it_interval.tv_sec = period_seconds;
                m_timerspec.it_value.tv_nsec = period_nanoseconds;
                m_timerspec.it_interval.tv_nsec = period_nanoseconds;

                return timer_settime(this->m_timer, 0, &m_timerspec, nullptr);
            }

            /**
             * @brief Callback function called when signal SIGNUM becomes pending for this process
             * 
             * @param _args Argument passsed to the callback function
             */
            void notify(void* _args){
                siginfo_t info;
                // sigset_t set = util::mask_signal(this->m_sigevent->sigev_signo);
                sigset_t set;
                sigaddset(&set, this->m_sigevent->sigev_signo);
                
                
                while(true){
                    sigwaitinfo(&set, &info);
                    
                    if(this->callback != nullptr){
                        (*this->callback)(_args);
                        
                    }
                }
            }

            /**
             * @brief Register a callback function that will be invoked on every clock signal
             * 
             * @tparam T Generic type of callback function
             * @param _callback Predicate or callback functoin
             */
            template<typename T>
            void onNotify(T _callback){
                
                static_assert(std::is_same< typename  callback_type<T>::template arguments<0>, void* >::value == true, "dsdfd");
                this->callback = new Callback(_callback);
            }

            ~Timer(){
                timer_delete(m_timer);
                delete m_sigevent;
                delete callback;
            }


        public:
            const static u_int16_t THOUSAND = 1000;
            const static u_int32_t MILLION = 1000000;

            itimerspec get_timer_spec(){
                return this->m_timerspec;
            }
            
        private:
            timer_t m_timer;
            int m_timer_id;
            struct sigevent* m_sigevent;
            Callback* callback;
            itimerspec m_timerspec;
            // unsigned char m_hyperperiod;
            // unsigned char m_cycles;

    };
}