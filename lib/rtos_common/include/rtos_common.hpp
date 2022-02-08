#pragma once

#include "rtos_callback.hpp"
#include "rtos_traits.hpp"

#define BUFFER_SIZE 1536


namespace rtos{
    //Using signed byte since data is provided in ASCII format
    typedef char BYTE;
}