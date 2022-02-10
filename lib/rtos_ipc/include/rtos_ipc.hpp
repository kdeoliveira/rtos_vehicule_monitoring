#pragma once

#include <signal.h>

namespace rtos{


    namespace util{
        static sigset_t mask_signal(const int _signmum){
            sigset_t set;
            sigemptyset(&set);

            sigaddset(&set, _signmum);
            sigprocmask(_signmum, &set, nullptr);

            return set;
        }
    }
}