
#include <cstdlib>
#include <rtos_pipe.hpp>
#include <rtos_data.hpp>
#include "common.hpp"



int main(int argc, char * argv[]){
    rtos::InputFile input_file("/home/kdeoliveira/dev/rtos_vehicule_monitoring/src/dataset.csv");

    if(argc != 3) exit(EXIT_FAILURE);
    int arg_fd[2] = {input_file.get_fd(), atoi(argv[2])};

    rtos::PipeManager pipe {arg_fd, rtos::PipeMode::READ, rtos::PipeFlag::REDIRECT};

    pipe.onRead( [](char* arg){
        std::cout << "onRead: " << arg << std::endl;
    });

    pipe.read_pipe<char*>();


    return EXIT_SUCCESS;
}