#pragma once

#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

#include <semaphore.h>

namespace rtos{

    template<typename T>
    class SharedMem{

        public:
            SharedMem(const char* name) : m_name{name} {
                this->m_fd = shm_open(m_name, O_CREAT | O_RDWR, S_IRUSR, S_IWUSR);

                if(m_fd == -1)  throw "Unable to open shared memory";

                if(ftruncate(this->m_fd, sizeof(T)) == -1){
                    throw "Unable to set size on file descriptor";
                }


                this->m_object = (T*) mmap(nullptr, sizeof(* this->m_object), PROT_READ | PROT_WRITE, MAP_SHARED, this->m_fd, 0);
                
            }






            ~SharedMem(){

            }


        private:
            int m_fd;
            const char* m_name;

            sem_t* m_semaphore;

            //Implement a wrapper instead
            T* m_object;

    };
}