#pragma once

/**
 * @brief Shared buffer used by shared memory segment to control the timer period
 * 
 */
struct _buffer_clock{
    int current_val_seconds = 1;
    int current_val_nanoseconds = 0;
};

typedef struct _buffer_clock buffer_clock;