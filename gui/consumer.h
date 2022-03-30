#ifndef CONSUMER_H
#define CONSUMER_H
#include <rtos_scheduler.hpp>
#include <rtos_algorithm.hpp>
#include <rtos_task.hpp>
#include <rtos_shared_mem.hpp>
#include "../src/common.hpp"


class ConsumerSchedulerAlgo : public rtos::algorithm<period_task>
{
public:
    ConsumerSchedulerAlgo(const int _sz) : rtos::algorithm<period_task>(_sz) {}

    template <typename T = period_task>
    void push(T &task)
    {
        this->m_queue[this->m_index++] = task;
    }

    template <typename T = period_task, typename... K>
    void push(T &task, K &...k)
    {
        this->push(k...);
        this->push(task);
    }

    void *run(rtos::timer_cycle *_timer_cycle, const int &_sig) const override
    {
           #define DEBUG

        #ifdef DEBUG
            printf("[consumer] Cycle: %u\n", _timer_cycle->cycles);
        #endif

            for (int i{0}; i < this->size(); i++)
            {
                if((_timer_cycle->cycles + 1) % this->m_queue[i].period == 0)
                {
                    #ifdef DEBUG
                        printf("[debug - consumer] period of task %u -> %u \n", _timer_cycle->cycles, this->m_queue[i].period);
                        printf("[consumer] ptask id: %lu\n", this->m_queue[i].thread_id);
                    #endif

                    pthread_kill(this->m_queue[i].thread_id, _sig);


                }
            }



        return nullptr;
    }
};


class FuelConsumption : public rtos::Task<char *>{
    public:
        FuelConsumption(const char* shared_name) : m_input_buffer{shared_name}{}
        void run() override{

            std::cout << this->m_input_buffer->buffer[SensorsHeader::Fuel_consumption].header.id << ": " << this->m_input_buffer->buffer->payload << std::endl;
        }

        ~FuelConsumption(){
        }

    private:
        std::mutex m_mx;
        // rtos::buffer<rtos::packet_data<SensorsHeader, SensorValue>>& m_input_buffer;
        rtos::SharedMem<buffer_packets> m_input_buffer;

};


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

class MainThread : public rtos::Thread<char*>{
    public:
        MainThread(int id, rtos::Task<char *>* task) : rtos::Thread<char *>{task, false, SIGUSR2}, m_id{id}{
            this->start();
        }

    private:
        int m_id;
};

#endif // CONSUMER_H
