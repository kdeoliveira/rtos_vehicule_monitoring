#include <rtos_scheduler.hpp>
#include <rtos_algorithm.hpp>
#include <rtos_task.hpp>
#include <rtos_shared_mem.hpp>
#include "common.hpp"

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
        if (_timer_cycle->cycles <= this->size())
        {

            for (int i{0}; i < this->size(); i++)
            {
                printf("[debug] period of task %u -> %u \n", _timer_cycle->cycles, this->m_queue[i].period);
                if (_timer_cycle->cycles == this->m_queue[i].period)
                {
                    printf("[producer] ptask id: %lu\n", this->m_queue[i].thread_id);
                    pthread_kill(this->m_queue[i].thread_id, _sig);

                    return nullptr;
                }
            }
        }

        return nullptr;
    }
};


class FuelConsumption : public rtos::Task<char *>{
    public:
        FuelConsumption(const char* shared_name) : m_input_buffer{shared_name}{}
        void run() override{

            puts("From fuel consumption");

            std::cout << m_input_buffer->buffer->at(SensorsHeader::Fuel_consumption) << std::endl;

            for(auto& x : *m_input_buffer->buffer){
                std::cout << x;
            }
            std::cout <<std::endl;            
        }

        ~FuelConsumption(){
            // delete m_packet_data;
            // delete m_packet_header;
        }

    private:
        std::mutex m_mx;
        // rtos::buffer<rtos::packet_data<SensorsHeader, SensorValue>>& m_input_buffer;
        rtos::SharedMem<buffer_packets> m_input_buffer;
        
};

class MainThread : public rtos::Thread<char*>{
    public:
        MainThread(int id, rtos::Task<char *>* task) : m_id{id}, rtos::Thread<char *>{task, false, SIGUSR2}{
        }

    private:
        int m_id;
};


int main(int argc, char *argv[])
{

    rtos::Task<char *>* consumer = new FuelConsumption("m_buffer_input");

    std::unique_ptr<MainThread> thread_consumer  = std::make_unique<MainThread>(5, consumer);


    thread_consumer->start();

    auto *algo = new ConsumerSchedulerAlgo{1};

    rtos::Scheduler sched_consumer{SIGUSR1, algo};

    period_task c_task[1];
    c_task[0].period = (uint8_t)2;
    c_task[0].thread_id = thread_consumer->get_thread_id();

    algo->push(c_task[0]);

    sched_consumer.dispatch(SIGUSR2);

    thread_consumer->join();

    return EXIT_SUCCESS;
}