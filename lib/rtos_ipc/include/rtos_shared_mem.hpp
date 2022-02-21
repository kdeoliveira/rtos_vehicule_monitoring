#pragma once

#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <memory>

namespace rtos
{

    template <typename T>
    class SharedMem
    {

    public:
        SharedMem(const char *name) : m_name{name}
        {

            this->m_fd = shm_open(m_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

            if (m_fd == -1)
                throw "Unable to open shared memory";

            if (ftruncate(this->m_fd, sizeof(T)) == -1)
            {
                throw "Unable to set size on file descriptor";
            }

            m_object = new T{};
           
            m_object = (T*) mmap(
                nullptr, sizeof(*m_object), PROT_READ | PROT_WRITE, MAP_SHARED, this->m_fd, 0);
            if (this->m_object == MAP_FAILED)
                perror("mmap");
        }

        ~SharedMem()
        {
            shm_unlink(this->m_name);

            munmap(m_object, sizeof(*m_object));
            close(this->m_fd);
        }

        T* operator->(){
            return m_object;
        }

        const int get_fd(){
            return this->m_fd;
        }


    private:
        T* m_object;
        int m_fd;
        const char *m_name;

        sem_t *m_semaphore;
    };
}