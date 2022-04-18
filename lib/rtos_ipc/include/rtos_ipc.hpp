#pragma once

#include <signal.h>
#include <stdio.h>

namespace rtos{


    namespace util{
        /**
         * @brief Helper function that masks a signal on the current process
         * 
         * @param _signmum SIGNAL to be masked
         * @return sigset_t 
         */
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