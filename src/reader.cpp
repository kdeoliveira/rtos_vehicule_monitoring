
#include <cstdlib>
#include <rtos_pipe.hpp>
#include <rtos_data.hpp>
#include "common.hpp"

#include <vector>

int main(int argc, char * argv[]){
    uid_t uid = geteuid();
    char* whoami = new char[10];
    getlogin_r(whoami, sizeof(char*));
    puts(whoami);
    
    char * filename = new char[70];
    sprintf(filename, "/home/%s/dev/rtos_vehicule_monitoring/src/dataset.csv", whoami);

    rtos::InputFile input_file(filename);

    std::vector<rtos::packet_data<SensorsHeader, float>> m_input_map; 

    if(argc != 3) exit(EXIT_FAILURE);
    int arg_fd[2] = {input_file.get_fd(), atoi(argv[2])};

    rtos::PipeManager pipe {arg_fd, rtos::PipeMode::READ, rtos::PipeFlag::REDIRECT};

    _V2::system_clock::time_point start, stop;


    pipe.onRead( [&](char* arg){
        static int m_arg_row;
        int m_arg_col = 0;

        start = high_resolution_clock::now();



        std::unique_ptr<rtos::packet_data<SensorsHeader, float>> m_packet = std::make_unique<rtos::packet_data<SensorsHeader, float>>();



        char* res;

        res = strtok(arg, ",");

        if(m_arg_row > 0){
            *m_packet = SensorsHeader(m_arg_col);
            *m_packet << (float) atof(res);
            m_input_map.push_back(*m_packet.get());
            // std::cout << *m_packet << std::endl;
        }

        
        while( (res = strtok(nullptr, ",")) != nullptr){
            ++m_arg_col;


            if(m_arg_row > 0){
                *m_packet = SensorsHeader(m_arg_col);
                *m_packet << atof(res);
                m_input_map.push_back(*m_packet.get());

            }
        }
        stop = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(stop - start);
        printf("Execution time: %ld ns\n", duration.count());

        m_arg_row++;
    });

    
    pipe.read_pipe<char*>();
    pipe.read_pipe<char*>();





    // for(auto& x : m_input_map){
    //     std::cout << x.first << std::endl;
    // }


    return EXIT_SUCCESS;
}