#pragma once

#include <signal.h>

namespace rtos{


    namespace util{
        static sigset_t mask_signal(const int _signmum){
            sigset_t set;
            sigemptyset(&set);

            sigaddset(&set, _signmum);
            if(sigprocmask(SIG_BLOCK, &set, nullptr) < 0){
                perror("sigprocmask");
                throw "Unable to mask signal for tihs thread";
            }

            return set;
        }
    }
}