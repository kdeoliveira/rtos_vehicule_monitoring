
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

int main(int argc, char *argv[])
{
    rtos::util::mask_signal(SIGUSR1);
    rtos::util::mask_signal(SIGUSR2);
    rtos::util::mask_signal(SIGALRM);



    pid_t main_sig;

    int fd[2];

    pid_t pid = fork();

    if(pid > 0){
        main_sig = pid;
    }

    if (pid == 0)
    {

        if (pipe(fd) < 0)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
        char arg_fd_1[2];
        char arg_fd_2[2];
        sprintf(arg_fd_1, "%d", fd[0]);
        sprintf(arg_fd_2, "%d", fd[1]);

        std::string path = get_current_dir_name();


        pid_t child_pid = fork();

        if(child_pid == 0){
            const char *arg_pid = std::to_string(getpid()).c_str();
            path += "/src/output";
            path ="gnome-terminal -e "+path;

            // if (execl(path.c_str(), arg_pid, arg_fd_1, arg_fd_2, NULL) < 0)
            // {
            //     perror("execl");
            // }


            std::system(path.c_str());
        _exit(EXIT_SUCCESS);

        }else{
            const char *arg_pid = std::to_string(getpid()).c_str();
        
            std::string path = get_current_dir_name();
            path += "/src/scheduling";

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

        rtos::Timer m_timer{CLOCK_REALTIME, SIGALRM};

        if (m_timer.start(2, 0) < 0)
            perror("timer_settime");

        m_timer.onNotify([&](void *val)
                         {
            sigval_t value;
            value.sival_int = 10;
            // kill(getppid(), SIGUSR1);
            // sigqueue(parent_pid, SIGUSR1, value);
        
            kill(0, SIGUSR1);
            // sigqueue(pid, SIGUSR1, value);

            

        });

        m_timer.notify(0, SIGALRM, nullptr);

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
