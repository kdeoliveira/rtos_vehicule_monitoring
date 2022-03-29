#pragma once

#include "rtos_callback.hpp"
#include "rtos_traits.hpp"

#define BUFFER_SIZE 1536


namespace rtos{
    //Using signed byte since data is provided in ASCII format
    typedef char BYTE;
}



#ifdef _QNX_x86_64
    #include "signal.h"
    typedef signed char int8_t;
    typedef unsigned char u_int8_t;
    typedef signed short int int16_t;
    typedef unsigned short int u_int16_t;
    typedef signed int int32_t;
    typedef unsigned int u_int32_t;


    typedef struct sigevent sigevent_t;
#endif