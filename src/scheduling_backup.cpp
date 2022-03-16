#include <rtos_scheduler.hpp>
#include <rtos_algorithm.hpp>
#include <rtos_timer.hpp>

#include <sys/types.h>
#include <sys/wait.h>
#include <rtos_task.hpp>
#include <rtos_buffer.hpp>

#include "common.hpp"

#include <rtos_packet.hpp>
#include <rtos_file.hpp>
#include <rtos_pipe.hpp>

#include <semaphore.h>


#include <rtos_shared_mem.hpp>


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

        if (start == 0)
        {
            clock_gettime(CLOCK_MONOTONIC, &tv);
            start = tv.tv_sec * rtos::Timer::THOUSAND + tv.tv_nsec / rtos::Timer::MILLION;
        }

        clock_gettime(CLOCK_MONOTONIC, &tv);
        current = tv.tv_sec * rtos::Timer::THOUSAND + tv.tv_nsec / rtos::Timer::MILLION;



        if (_timer_cycle->cycles > 0)
        {
            printf("Current time spend: %f\n",  (double)(current - start));
        }


        printf("Cycle: %u\n", _timer_cycle->cycles);

        if(_timer_cycle->cycles < m_index){
            printf("ptask id: %lu\n", this->m_queue[(int)_timer_cycle->cycles].thread_id);
            pthread_kill(this->m_queue[_timer_cycle->cycles].thread_id, _signmum);
        }
        return nullptr;
    }
};



std::condition_variable m_cd;
bool ready = true;

class FuelConsumption : public rtos::Task<char *>{
    public:
        FuelConsumption(const char* shared_name) : m_input_buffer{shared_name}{}
        void run() override{

            puts("From fuel consumption");

            std::cout << m_input_buffer->buffer.at(SensorsHeader::Fuel_consumption) << std::endl;

            for(auto& x : m_input_buffer->buffer){
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

class Producer : public rtos::Task<char *>{
    public:
    Producer(const char* shared_name, int arg_fd[2]) : pipe {arg_fd, rtos::PipeMode::READ, rtos::PipeFlag::REDIRECT}, m_input_buffer{shared_name}{
    using _type = typename std::remove_pointer<decltype(m_input_buffer->buffer)>::type;
    m_input_buffer->buffer = _type(54);

    pipe.onRead( [&](char* arg){
        static int m_arg_row;
        int m_arg_col = 0;




        std::unique_ptr<rtos::packet_data<SensorsHeader, SensorValue>> m_packet = std::make_unique<rtos::packet_data<SensorsHeader, SensorValue>>();


        char* res;

        res = strtok(arg, ",");

        if(m_arg_row > 0){
            *m_packet = SensorsHeader(m_arg_col);
            *m_packet << (float) atof(res);
            m_input_buffer->buffer.add(*m_packet, m_arg_col);

        }

        
        while( (res = strtok(nullptr, ",")) != nullptr){
            ++m_arg_col;


            if(m_arg_row > 0){
                *m_packet = SensorsHeader(m_arg_col);
                if(m_arg_col == 52){
                    *m_packet << static_cast<float>(*res);

                }else{
                    *m_packet << (float) atof(res);
                }
                m_input_buffer->buffer.add(*m_packet, m_arg_col);

            }
        }
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
        MainThread(int id, rtos::Task<char *>* task) : m_id{id}, rtos::Thread<char *>{task, false, SIGUSR2}{
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

    

    uid_t uid = geteuid();
    char whoami[20];
    getlogin_r(whoami, sizeof(char*)*20);
    
    char * filename = new char[70];
    sprintf(filename, "/home/qnxuser/data/dataset.csv", whoami);

    

    rtos::InputFile input_file(filename);

    // if(argc != 3) exit(EXIT_FAILURE);
    int temp_fd[2];
    pipe(temp_fd);

    int arg_fd[2] = {input_file.get_fd(), temp_fd[1]};


    // rtos::buffer<rtos::packet_data<SensorsHeader, SensorValue>> m_buffer_input(54);


    


    pid_t pid = fork();

    if (pid < 0)
        return -1;

    if (pid == 0)
    {
    std::cout << "HERE CALLED" << std::endl;


        rtos::Timer m_timer{CLOCK_REALTIME, SIGUSR1};

        if (m_timer.start(2, 0) < 0)
            perror("timer_settime");

        m_timer.onNotify([&](void *val)
                         {
            sigval_t value;
            value.sival_int = 10;
            // kill(getppid(), SIGUSR1);
            sigqueue(getppid(), SIGUSR1, value);


            
            
            
            });


        m_timer.notify(0, SIGUSR1, nullptr);

        _exit(EXIT_SUCCESS);
    }
    else
    {

        rtos::Task<char*>* fc = new FuelConsumption("m_buffer_input");
        rtos::Task<char*>* pd = new Producer("m_buffer_input", arg_fd);


        std::unique_ptr<MainThread> thread = std::make_unique<MainThread>(5, fc);
        std::unique_ptr<MainThread> thread1 = std::make_unique<MainThread>(10, pd);

        thread->start();
        thread1->start();




        auto *algo = new ProducerSchedulerAlgo{2};
        rtos::Scheduler sched{SIGUSR1, algo};

        period_task p_task[2];
        p_task[0].period = (uint8_t) 2;
        p_task[0].thread_id = thread->get_thread_id();


        p_task[1].period = (uint8_t) 1;
        p_task[1].thread_id = thread1->get_thread_id();

        algo->push(p_task[0], p_task[1]);

        //TOOD: !!Check if sigwaitinfo() indeed stop process!!
        sched.dispatch(SIGUSR2);

        thread->join();
        thread1->join();




        _exit(EXIT_SUCCESS);

    }
    }
    catch(std::exception &e ){
        puts(e.what());
    }

    return EXIT_SUCCESS;
}