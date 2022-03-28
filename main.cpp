
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

#include <sys/resource.h>
#include <sys/mman.h>

#include <semaphore.h>

void signal_handler(int signum){

    std::cout << "=======attempting to gracefully stop current process=======" << std::endl;

    sem_unlink("sem_modification");
    sem_unlink("sem_access");
    shm_unlink("m_buffer_input");


    

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

int main(int argc, char *argv[])
{
    //Gracefully closing all opened fds if SIGINT signal event occurs
    signal(SIGINT, signal_handler);

    rtos::util::mask_signal(SIGUSR1);
    rtos::util::mask_signal(SIGUSR2);
    // rtos::util::mask_signal(SIGALRM);


    puts("Application starting...");

    
    int fd[2];

    pid_t pid = fork();


        if (pipe(fd) < 0)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

    if (pid == 0)
    {

        char arg_fd_1[2];
        char arg_fd_2[2];
        sprintf(arg_fd_1, "%d", fd[0]);
        sprintf(arg_fd_2, "%d", fd[1]);


        char buf_temp[PATH_MAX + 1];

        getcwd(buf_temp, PATH_MAX + 1);


        std::string path = buf_temp;


        pid_t child_pid = fork();

        if(child_pid == 0){


            const char *arg_pid = std::to_string(getpid()).c_str();
            #ifdef _QNX_x86_64
                path += "/gui/qnx/debug/gui";
            #else
                path += "/gui/x64/debug/gui";
            #endif


            if (execl(path.c_str(), arg_pid, arg_fd_1, arg_fd_2, NULL) < 0)
            {
                perror("execl");
            }

        _exit(EXIT_SUCCESS);

        }else{



            const char *arg_pid = std::to_string(getpid()).c_str();

            char buf_temp[PATH_MAX + 1];

            getcwd(buf_temp, PATH_MAX + 1);

            std::string path = buf_temp;
            path += "/src/producer";


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
        
        rtos::Timer m_timer{CLOCK_REALTIME, SIGUSR2};

        if (m_timer.start(0, rtos::Timer::MILLION*25) < 0)
            perror("timer_settime");

        m_timer.onNotify([&](void *val){
            killpg( getpgid(pid) , SIGUSR1);
        });


        m_timer.notify(0, nullptr);

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
