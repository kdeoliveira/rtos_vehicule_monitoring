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

    /**
     * @brief POSIX shared memory API allows processes to communicate information by sharing a region of memory.
     * 
     * @tparam T type of object that will be stored
     */
    template <typename T>
    class SharedMem
    {

    public:
        /**
         * @brief Construct a new shared memory segment and creates appropriate mapping in the virtual address space
         * 
         * @param name name of the shared memory instance created
         */
        SharedMem(const char *name) : m_name{name}
        {

            this->m_fd = shm_open(m_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

            if (m_fd == -1)
                throw "Unable to open shared memory";

            if (ftruncate(this->m_fd, sizeof(T)) == -1)
            {
                throw "Unable to set size on file descriptor";
                perror("ftruncate");
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

        /**
         * @brief Access to the object stored in shared memory
         * 
         * @return T* pointer to the object's value
         */
        T* operator->(){
            return m_object;
        }

        /**
         * @brief Get the file descriptor used by shm_open
         * 
         * @return int 
         */
        int get_fd() const{
            return this->m_fd;
        }


    private:
        T* m_object;
        int m_fd;
        const char *m_name;
    };
}