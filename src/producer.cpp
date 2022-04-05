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

#ifndef _QNX_x86_64
    #include <sys/syscall.h>

    #ifndef SYS_gettid
        #error "SYS_gettid unavailable on this system"
    #endif

    #define gettid() ((pid_t)syscall(SYS_gettid))
#endif







/**
 * @brief First task executed by the producer responsible for periodically reading all rows of the dataset.csv. This ensures that each row of the dataset is read at a given period, regardless of other producer tasks defined.
 * A pipe is used in redirect mode in order to properly read the content of the dataset file
 * Note that although each task is executed in a given period each thread should make use of process synchronization techniques to avoid race condition
 */
class Producer : public rtos::Task<char *>{
    public:
    Producer(const char* shared_name, int arg_fd[2]) : pipe {arg_fd, rtos::PipeMode::READ, rtos::PipeFlag::REDIRECT}, m_input_buffer{shared_name}{
    m_input_buffer->semaphore_modification = sem_open("/sem_modification", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1);

    using _type = typename std::remove_pointer<decltype(m_input_buffer->buffer)>::type;

    if(m_input_buffer->status != 1){
        m_input_buffer->status = 1;
    }

    /**
     * @brief Uses the read end of file descriptor genereted by the InputFile to read each line at the times
     * This function is responsible for properly parsing each column and storing inside a buffer
     */
    pipe.onRead( [&](char* arg){
        static int m_arg_row;
        int m_arg_col = 0;

        rtos::packet_data<SensorsHeader, SensorValue> m_packet{};

        char* res;

        if( sem_wait(m_input_buffer->semaphore_modification) == -1 ){
            perror("sem_wait");
        }

        res = strtok(arg, ",");
        
        if(m_arg_row > 0){
            m_packet = SensorsHeader(m_arg_col);
            m_packet << (float) atof(res);

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

        
        #ifdef DEBUG
            std::cout << ">> Row pushed from producer: " << m_arg_row << std::endl;
        #endif
        m_arg_row++;

    if( sem_post(m_input_buffer->semaphore_modification) == -1 ){
        perror("sem_post");
    }


    });
    }

    ~Producer(){
        m_input_buffer->status = 0;
        sem_close(m_input_buffer->semaphore_access);
        sem_close(m_input_buffer->semaphore_modification);
        
    }
    void run() override{
            if(pipe.read_pipe<char*>() < 0){
                pthread_cancel(gettid());
            }
    }
    private:
        std::mutex m_mx;
        rtos::PipeManager pipe;
        rtos::SharedMem<buffer_packets> m_input_buffer;



};


/**
 * @brief Producer process responsible for periodically generating the required data provided in the dataset file
 * 
 * @param argc 
 * @param argv file descriptors opened by the parent process
 * @return int 
 */
int main(int argc, char *argv[])
{
    try{

    uid_t uid = geteuid();
    char whoami[20];
    getlogin_r(whoami, sizeof(char*)*20);
    
    char filename[1024] = DATASET_PATH;

    /**
     * @brief Opens a new instance of the dataset file
     * 
     * @return rtos::InputFile 
     */
    rtos::InputFile input_file(filename);

    if(argc != 3) exit(EXIT_FAILURE);
    int temp;
    sscanf(argv[2], "%d", &temp);
    int arg_fd[2] = {input_file.get_fd(), temp};

        puts("Starting producer task");
        

        rtos::Task<char*>* pd = new Producer("m_buffer_input", arg_fd);


        std::unique_ptr<MainThread> thread1 = std::make_unique<MainThread>(10, pd);

        /**
         * @brief Starts a new thread for the main producer task
         * 
         */
        thread1->start();

        /**
         * @brief Starts a new scheduler used by this producer process
         * Note that this scheduler is expected to receive a SIGUSR1 signal from the parent process before executing the scheduling algorithm
         * 
         */
        auto *algo = new SchedulerAlgo{1, "producer"};
        rtos::Scheduler<period_task> sched{SIGUSR1, algo, 5};

        period_task p_task[1];

        p_task[0].period = (uint8_t) 1;
        p_task[0].thread_id = thread1->get_thread_id();

        algo->push(p_task[0]);
        puts("Dispatching producer scheduler...");
        
        /**
         * @brief pthread_kill signal SIGNUM
         * 
         */
        sched.dispatch(SIGUSR2);

        thread1->join();


    }
    catch(std::exception &e ){
        puts(e.what());
    }
    catch(char const* x){
        puts(x);
    }
    puts("End of producer");
    return EXIT_SUCCESS;
}