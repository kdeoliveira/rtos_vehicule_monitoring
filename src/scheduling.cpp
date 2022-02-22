#include <rtos_scheduler.hpp>
#include <rtos_algorithm.hpp>
#include <rtos_timer.hpp>

#include <sys/types.h>
#include <sys/wait.h>
#include <rtos_task.hpp>

#include "common.hpp"

#include <rtos_file.hpp>
#include <rtos_pipe.hpp>

#include <semaphore.h>

#include <algorithm>


#include <rtos_shared_mem.hpp>


#include <cstring>




class ProducerSchedulerAlgo : public rtos::algorithm<period_task>
{
public:
    ProducerSchedulerAlgo(const int _sz) : rtos::algorithm<period_task>{_sz} {}

    template<typename T = period_task>
    void push(T& task)
    {
        this->m_queue[this->m_index++] = task;
    }

    template<typename T = period_task , typename...K>
    void push(T& task, K&...k)
    {
        this->push(k...);
        this->push(task);
    }

    void *run(rtos::timer_cycle* _timer_cycle, const int &_signmum) const override
    {
        // if(this->m_index == 0) return nullptr;
        static int start;
        int current;
        struct timespec tv;

        // if (start == 0)
        // {
        //     clock_gettime(CLOCK_MONOTONIC, &tv);
        //     start = tv.tv_sec * rtos::Timer::THOUSAND + tv.tv_nsec / rtos::Timer::MILLION;
        // }

        // clock_gettime(CLOCK_MONOTONIC, &tv);
        // current = tv.tv_sec * rtos::Timer::THOUSAND + tv.tv_nsec / rtos::Timer::MILLION;



        // if (_timer_cycle->cycles > 0)
        // {
        //     printf("Current time spend: %f\n",  (double)(current - start));
        // }


        printf("[producer] Cycle: %u\n", _timer_cycle->cycles);

            


            //kill thread by period value provided
            for(int i{0}; i < this->size() ; i++){
                printf("[debug - producer] period of task %u -> %u \n", _timer_cycle->cycles, this->m_queue[i].period);
                if(_timer_cycle->cycles == this->m_queue[i].period){
                    printf("[producer] ptask id: %lu\n", this->m_queue[i].thread_id);
                    pthread_kill(this->m_queue[i].thread_id, _signmum);

                    return nullptr;
                }
            }


            //kill thread by order of queue
            // if(this->m_queue[(int)_timer_cycle->cycles].period == _timer_cycle->cycles){
            //     printf("[producer] ptask id: %lu\n", this->m_queue[(int)_timer_cycle->cycles].thread_id);
            //     pthread_kill(this->m_queue[_timer_cycle->cycles].thread_id, _signmum);
            // }
        return nullptr;
    }
};



class Producer : public rtos::Task<char *>{
    public:
    Producer(const char* shared_name, int arg_fd[2]) : pipe {arg_fd, rtos::PipeMode::READ, rtos::PipeFlag::REDIRECT}, m_input_buffer{shared_name}{

    using _type = typename std::remove_pointer<decltype(m_input_buffer->buffer)>::type;

    // m_input_buffer->buffer = _type{54};
    // m_input_buffer->buffer = new _type[];
    m_input_buffer->status = 10;


    pipe.onRead( [&](char* arg){
        static int m_arg_row;
        int m_arg_col = 0;

        rtos::packet_data<SensorsHeader, SensorValue> m_packet{};

        char* res;

        res = strtok(arg, ",");

        if(m_arg_row > 0){
            m_packet = SensorsHeader(m_arg_col);
            m_packet << (float) atof(res);

            // std::memcpy(this->m_input_buffer->buffer.begin(), (const void *)m_packet.get(), sizeof(m_packet));

            std::strcpy(this->m_input_buffer->temp_buffer, res);

            m_input_buffer->buffer[m_arg_col] = m_packet;

        }
        
        while( (res = strtok(nullptr, ",")) != nullptr){
            ++m_arg_col;

            if(m_arg_row > 0){
                m_packet = SensorsHeader(m_arg_col);
                if(m_arg_col == 52){
                    m_packet << static_cast<float>(*res);
                }else{
                    m_packet << (float) atof(res);
                }
                this->m_input_buffer->buffer[m_arg_col] = m_packet;

            }
        }

        m_input_buffer->status = m_input_buffer->status  + 1;
        m_arg_row++;

    });
    }
    void run() override{
            std::cout << "Pushing to queue" << std::endl;

            if(pipe.read_pipe<char*>() < 0){
                pthread_cancel(gettid());
            }
    }
    private:
        std::mutex m_mx;
        rtos::PipeManager pipe;
        // rtos::buffer<rtos::packet_data<SensorsHeader, SensorValue>>& m_input_buffer;
        rtos::SharedMem<buffer_packets> m_input_buffer;



};


class MainThread : public rtos::Thread<char*>{
    public:
        MainThread(int id, rtos::Task<char *>* task) : m_id{id}, rtos::Thread<char *>{task, false, SIGUSR2}{    //Sensible to SIGUSR2 signal
        }

    private:
        int m_id;
};







 // =================== MAIN ================
 //
 //
int main(int argc, char *argv[])
{
    try{

    
    // {
    //     rtos::buffer<char> test_buffer{10};

    //     test_buffer[0] = 'K';

    //     rtos::buffer<char> test2_buffer = std::move(test_buffer);

    //     std::cout << test2_buffer[0] << std::endl;

    // }
    



    uid_t uid = geteuid();
    char whoami[20];
    getlogin_r(whoami, sizeof(char*)*20);
    
    char * filename = new char[70];
    sprintf(filename, "/home/%s/dev/rtos_vehicule_monitoring/src/dataset.csv", whoami);

    

    rtos::InputFile input_file(filename);

    if(argc != 3) exit(EXIT_FAILURE);
    int arg_fd[2] = {input_file.get_fd(), atoi(argv[2])};


    // rtos::buffer<rtos::packet_data<SensorsHeader, SensorValue>> m_buffer_input(54);


        puts("Starting producer task");
        

        // rtos::Task<char*>* fc = new FuelConsumption("m_buffer_input");
        rtos::Task<char*>* pd = new Producer("m_buffer_input", arg_fd);


        // std::unique_ptr<MainThread> thread = std::make_unique<MainThread>(5, fc);
        std::unique_ptr<MainThread> thread1 = std::make_unique<MainThread>(10, pd);

        // thread->start();
        thread1->start();




        auto *algo = new ProducerSchedulerAlgo{1};
        rtos::Scheduler sched{SIGUSR1, algo};

        period_task p_task[1];
        // p_task[0].period = (uint8_t) 2;
        // p_task[0].thread_id = thread->get_thread_id();


        p_task[0].period = (uint8_t) 1;
        p_task[0].thread_id = thread1->get_thread_id();

        algo->push(p_task[0]);
        puts("Dispatching producer scheduler...");
        
        //pthread_kill signal SIGNUM
        sched.dispatch(SIGUSR2);

        // thread->join();
        thread1->join();





    }
    catch(std::exception &e ){
        puts(e.what());
    }
    puts("End of schedulng");
    return EXIT_SUCCESS;
}