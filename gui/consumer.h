#ifndef CONSUMER_H
#define CONSUMER_H
#include <rtos_scheduler.hpp>
#include <rtos_algorithm.hpp>
#include <rtos_task.hpp>
#include <rtos_shared_mem.hpp>
#include "../src/common.hpp"

class SensorDataTask : public rtos::Task<char *>{
    public:
        SensorDataTask(const char* shared_name, u_int8_t header) : m_input_buffer{shared_name}, m_header{new SensorsHeader{(SensorsHeader)header} }{
            m_input_buffer->semaphore_access = sem_open("/sem_access", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1);
            // m_input_buffer->semaphore_modification = sem_open("/sem_modification", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 2);
        }


        ~SensorDataTask(){

            sem_close(m_input_buffer->semaphore_access);
            sem_close(m_input_buffer->semaphore_modification);
            delete m_header;
        }

        void run() override{




            if( sem_wait(m_input_buffer->semaphore_access) == -1 ){
                perror("sem_wait");
            }

            if(this->m_input_buffer->buffer[*m_header].header.size > 0)
                std::cout << this->m_input_buffer->buffer[*m_header].header.id << ": " << this->m_input_buffer->buffer[*m_header].payload << std::endl;

                // for(auto& e : this->m_input_buffer->buffer){
                //     std::cout << e;
                // }
                // std::cout << std::endl;

            if( sem_post(m_input_buffer->semaphore_access) == -1 ){
                perror("sem_post");
            }


        }


    private:
        rtos::SharedMem<buffer_packet> m_input_buffer;
        SensorsHeader* m_header;
};

class QMainThread : public rtos::Thread<char*>{
    public:
        QMainThread(int id, rtos::Task<char *>* task) : rtos::Thread<char *>{task, false, SIGUSR2}, m_id{id}{
            this->start();
        }

    private:
        int m_id;
};

#endif // CONSUMER_H
