
#include <cstdlib>
#include <rtos_pipe.hpp>
#include <rtos_data.hpp>
#include "common.hpp"

#include <rtos_shared_mem.hpp>

#include <rtos_buffer.hpp>




struct shmem_obj{
    char buffer[10];
};

typedef struct shmem_obj shmem;

int main(int argc, char * argv[]){
    uid_t uid = geteuid();
    char whoami[20];
    getlogin_r(whoami, sizeof(char*)*20);
    
    char * filename = new char[70];
    sprintf(filename, "/home/%s/dev/rtos_vehicule_monitoring/src/dataset.csv", whoami);

    
    puts("starting app ...");

    {

        rtos::SharedMem<shmem> shared_memory{"test_mem"};

        shared_memory->buffer[0] = 'k';
        shared_memory->buffer[1] = 'e';

        std::cout << shared_memory->buffer << std::endl;

    }


    rtos::InputFile input_file(filename);

    std::vector<rtos::packet_data<SensorsHeader, float>> m_input_map; 

    rtos::buffer<rtos::packet_data<SensorsHeader, SensorValue>> m_buffer_input(54);

    if(argc != 3) exit(EXIT_SUCCESS);
    int arg_fd[2] = {input_file.get_fd(), atoi(argv[2])};

    rtos::PipeManager pipe {arg_fd, rtos::PipeMode::READ, rtos::PipeFlag::REDIRECT};

    _V2::system_clock::time_point start, stop;


    pipe.onRead( [&](char* arg){
        static int m_arg_row;
        int m_arg_col = 0;

        start = high_resolution_clock::now();



        std::unique_ptr<rtos::packet_data<SensorsHeader, SensorValue>> m_packet = std::make_unique<rtos::packet_data<SensorsHeader, SensorValue>>();


        char* res;

        res = strtok(arg, ",");

        if(m_arg_row > 0){
            *m_packet = SensorsHeader(m_arg_col);
            *m_packet << (float) atof(res);
            m_buffer_input.add(*m_packet, m_arg_col);
        }

        
        while( (res = strtok(nullptr, ",")) != nullptr){
            ++m_arg_col;


            if(m_arg_row > 0){
                *m_packet = SensorsHeader(m_arg_col);
                if(m_arg_col == 52){
                    *m_packet << static_cast<float>(*res);
                    // *m_packet << static_cast<float>(*res;
            // std::cout << "VAL: " << static_cast<char>( static_cast<float>(*res) ) << std::endl;


                }else{
                    *m_packet << (float) atof(res);
                }
                m_buffer_input.add(*m_packet, m_arg_col);
            }
        }
        stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start);
        printf("Execution time: %ld ns\n", duration.count());

        m_arg_row++;
    });

    
    pipe.read_pipe<char*>();





    for(auto& x : m_buffer_input){
        std::cout << x;
    }
    std::cout << std::endl;


    return EXIT_SUCCESS;
}