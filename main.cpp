#include <stdio.h>
#include <cstdlib>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fcntl.h>
#include <rtos_ipc.hpp>
#include <rtos_timer.hpp>
#include <limits.h>

#include <rtos_shared_mem.hpp>

#include <sys/resource.h>
#include <sys/mman.h>

#include <semaphore.h>

#include <libgen.h>

#include <string.h>



struct _buffer_clock{
    int current_val_seconds = 1;
    int current_val_nanoseconds = 0;
};

typedef struct _buffer_clock buffer_clock;




/*!
 * @brief Signal handler called on interruption. Attempts to gracefully stop all running processes and unlinking any opened shared memory segment
 * 
 * @param signum 
 */
void signal_handler(int signum){

    #ifdef DEBUG
        std::cout << "=======attempting to gracefully stop current process=======" << std::endl;
    #endif

    sem_unlink("sem_modification");
    sem_unlink("sem_access");
    shm_unlink("m_buffer_input");
    shm_unlink("m_input_buffer_clock");



    

    struct rlimit limit_fd;
    
    if (getrlimit(RLIMIT_NOFILE, &limit_fd) != 0){
        perror("getrlimit");
        exit(EXIT_FAILURE);
    }

    for( unsigned long i{0} ; i < limit_fd.rlim_cur ; i++){
        int k = fcntl(i, F_GETFD);
        if(k != -1){
            // std::cout << "Closing opened file descriptors" << std::endl;
            if( close(i) != 0){
                perror("close");
            }
        }
    }

    exit(EXIT_SUCCESS);
}



/*!
 * Process manager responsible for starting the Consumer and Producer tasks
 * Each task is run by a new child process:
 * Consumer: /src/consumer.cpp or /gui/main.cpp
 * Producer: /src/producer.cpp
 * 
 * A second child process is created exclusively for the Timer
 * Define TERMINAL if consumer is to be outputted into the terminal instead
 */
int main(int argc, char *argv[])
{

    #ifdef DEBUG
        puts("DEBUG MODE");
    #endif
    //Gracefully closing all opened fds if SIGINT signal event occurs
    signal(SIGINT, signal_handler);

    /*!
     * @brief Mask all signals used by this application
     * 
     */
    rtos::util::mask_signal(SIGUSR1);
    rtos::util::mask_signal(SIGUSR2);
    // rtos::util::mask_signal(SIGALRM);


    puts("Application starting...");


    /*!
     * @brief Gets the current working directory
     * 
     */
    char res[PATH_MAX];
    ssize_t cnt = readlink("/proc/self/exe", res, PATH_MAX);
    char buf_temp[PATH_MAX];
    if(cnt != -1){
        strcpy(buf_temp, dirname(res));
    }else{
        getcwd(buf_temp, PATH_MAX + 1);
    }

    
    int fd[2];
    if (pipe(fd) < 0)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();



    if (pid == 0)
    {

        char arg_fd_1[2];
        char arg_fd_2[2];
        sprintf(arg_fd_1, "%d", fd[0]);
        sprintf(arg_fd_2, "%d", fd[1]);
       
        std::string path = buf_temp;


        pid_t child_pid = fork();

        if(child_pid == 0){


            const char *arg_pid = std::to_string(getpid()).c_str();

            #ifdef TERMINAL
                path += "/src/consumer";
            #else

                #ifdef DEBUG
                    #ifdef _QNX_x86_64
                        path += "/gui/qnx/debug/gui";
                    #else
                        path += "/gui/x64/debug/gui";
                    #endif
                #else
                    #ifdef _QNX_x86_64
                        path += "/gui/qnx/release/gui";
                    #else
                        path += "/gui/x64/release/gui";
                    #endif
                #endif
            #endif



            /**
             * @brief Starts the consumer process and passes all required arguments
             * 
             */
            if (execl(path.c_str(), arg_pid, arg_fd_1, arg_fd_2, NULL) < 0)
            {
                perror("execl");
            }

        _exit(EXIT_SUCCESS);

        }else{



            const char *arg_pid = std::to_string(getpid()).c_str();

            std::string path = buf_temp;
            path += "/src/producer";

            /**
             * @brief Starts the producer process and passes all the required arguments
             * 
             */
            if (execl(path.c_str(), arg_pid, arg_fd_1, arg_fd_2, NULL) < 0)
            {
                perror("execl");
            }
        _exit(EXIT_SUCCESS);

        }


        _exit(EXIT_SUCCESS);
    }

    else
    {

        rtos::SharedMem<buffer_clock> shared_mem_timer("m_input_buffer_clock");

        shared_mem_timer->current_val_seconds = 1;
        shared_mem_timer->current_val_nanoseconds = 0;

        
        rtos::Timer m_timer{CLOCK_MONOTONIC, SIGUSR2};


        // if (m_timer.start(0, rtos::Timer::MILLION*25) < 0)
        if (m_timer.start(1, 0) < 0)
            perror("timer_settime");

        m_timer.onNotify([&](void *val){
            if(m_timer.get_timer_spec().it_value.tv_sec != shared_mem_timer->current_val_seconds || m_timer.get_timer_spec().it_value.tv_nsec != shared_mem_timer->current_val_nanoseconds){

                m_timer.start(
                    shared_mem_timer->current_val_seconds, shared_mem_timer->current_val_nanoseconds
                );

                #ifdef DEBUG
                    std::cout << "\033[1;41mValue of timer changed\33[0m" << std::endl;
                    std::cout << shared_mem_timer->current_val_seconds << " s \t " << shared_mem_timer->current_val_nanoseconds << " ns" << std::endl;
                #endif

            }
            //Signal parent process every cycle
            //to wake-up the Scheduler
            killpg( getpgid(pid) , SIGUSR1);
        });


        m_timer.notify(nullptr);

        int status;
        if (waitpid(pid, &status, 0) > 0)
        {
            if (WIFEXITED(status) && !WEXITSTATUS(status))
                printf("child program exited successfully\n");
            else if (WIFEXITED(status) && WEXITSTATUS(status))
            {
                if (WEXITSTATUS(status) == 127)
                    printf("execv failed\n");
                else
                    printf("child program exit failed\n");
            }
            else
            {
                printf("child program didn't terminated normally\n");
            }
        }
        else
        {
            perror("waitpid");
        }
        _exit(EXIT_SUCCESS);
    }

    return EXIT_SUCCESS;
}
