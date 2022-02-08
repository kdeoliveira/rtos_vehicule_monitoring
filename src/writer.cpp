#include <rtos_parser.hpp>
#include <rtos_file.hpp>
#include <rtos_pipe.hpp>
#include <sys/wait.h>
#include <iostream>
#include <queue>
#include <string>
#include "common.hpp"
#include <thread>

#define GET_VAR(name) (#name)

#include <mutex>
#include <vector>
int input_size{0};

template <typename T, typename K>
class RunnableOut
{
public:
    RunnableOut(const RunnableOut &) = delete;
    RunnableOut(std::deque<T> &arg, std::deque<K> &arg2) : in{arg}, out{arg2}
    {
    }

    void run()
    {

        if (input_size == 1)
        {

            this->msg = map_sensors[strtok(in.front(), ",")];
        }
        else
        {
            this->msg << (float)atof(
                strtok(in.front(), ","));
        }
        in.pop_front();

        this->out.push_back(msg);

        char *res;

        while (true)
        {

            std::unique_lock<std::mutex> ul(this->mx);

            if ((res = strtok(nullptr, ",")) != nullptr)
            {
                if (input_size == 1)
                    this->msg = map_sensors[res];
                else
                    this->msg << (float)atof(strtok(this->in.front(), ","));
            }

            if (!in.empty())
            {
                in.pop_front();
            }
            out.push_back(msg);

            // if(input_size == 30) break;
        }
    }

private:
    std::mutex mx;
    std::mutex mx_out;

    std::deque<T> &in;
    std::deque<K> &out;
    K msg;
};

template <typename T>
class RunnableInputFile
{
public:
    RunnableInputFile(const RunnableInputFile &) = delete;
    RunnableInputFile(std::deque<T> &arg) : in{arg}, input{"/home/kdeoliveira/dev/rtos_vehicule_monitoring/src/dataset.csv"}
    {
    }

    void run()
    {
        while (true)
        {
            std::unique_lock ul{this->mx};

            in.push_back(input.read_line());
            input_size++;

            if (input_size == 30)
                break;
        }
    }

private:
    std::deque<T> &in;
    std::mutex mx;
    rtos::InputFile input;
};

rtos::packet_data<SensorsHeader, float> *msg = new rtos::packet_data<SensorsHeader, float>();

std::vector<SensorsHeader> msg_headers;

void fn(std::deque<rtos::packet_data<SensorsHeader, float>> &queue, char *line, int input_file_size)
{

    int index = 0;

    if (input_file_size == 1)
    {
        msg_headers.emplace_back(map_sensors[strtok(line, ",")]);
    }
    else
    {
        *msg = msg_headers.at(index);
        *msg << (float)atof(strtok(line, ","));
        
    }

    // queue.push_back(*msg);

    // char *res;
    // while(
    //     (  res = strtok(nullptr, ",") )  != nullptr){
    //     index++;

    //     if(input_file_size == 1){
    //         msg_headers.emplace_back ( map_sensors[ res ] );
    //     }else{
    //         *msg = msg_headers.at(index);
    //         *msg << (float)atof(res);
    //     }

    //     queue.push_back(*msg);
    // }
}

int main(int argc, char *argv[])
{

    rtos::packet_data<SensorsHeader, float> packet_payload{};

    packet_payload.header = SensorsHeader::Throttle_position_signal;

    packet_payload << 127.1f << 23.1f;

    std::cout << packet_payload.payload << std::endl;

    std::cout << packet_payload.size() << std::endl;

    std::cout << atoi(argv[0]) << atoi(argv[1]) << atoi(argv[2]) << std::endl;

    // std::deque<rtos::packet_data<SensorsHeader, float>> output;
    // std::deque<char *> input;
    // RunnableOut<char *, rtos::packet_data<SensorsHeader, float>> runnableOut(std::ref(input), std::ref(output));

    // RunnableInputFile<char *> runnableInputFile(std::ref(input));

    // std::thread thread_1 {&RunnableInputFile<char *>::run, std::ref(runnableInputFile)};
    // std::thread thread_2 {&RunnableOut<char *, rtos::packet_data<SensorsHeader, float>>::run, std::ref(runnableOut)};

    // thread_1.join();
    // thread_2.join();

    rtos::InputFile input_file("/home/kdeoliveira/dev/rtos_vehicule_monitoring/src/dataset.csv");

    char *line = new char[35];

    std::deque<rtos::packet_data<SensorsHeader, float>> queue;
    // if(input_file.size() == 1){
    //     msg =  map_sensors[ strtok(line, ",") ];
    // }else{
    //     msg << (float) atof(
    //         strtok(line, ",")
    //     );
    // }
    // queue.push_back(msg);

    // char *res;

    // while(
    //     (  res = strtok(nullptr, ",") )  != nullptr){

    //     if(input_file.size() == 1){
    //         msg = map_sensors[ res ];

    //     }else{
    //         msg << (float)atof(res);
    //     }
    //     queue.push_back(msg);
    // }

    _V2::system_clock::time_point start, stop;

    if (argc != 3)
        exit(EXIT_FAILURE);
    int arg_fd[2] = {atoi(argv[1]), atoi(argv[2])};

    rtos::PipeManager pipe{arg_fd, rtos::PipeMode::WRITE, rtos::PipeFlag::REDIRECT};

    // while (input_file.get_file_size() > input_file.get_position()){
    // while(input_size < 10){
    //     start = high_resolution_clock::now();
    //     input_size++;
    //     // fn(queue, input_file.read_line(), input_size);

        
    //     pipe.write_pipe(
    //          input_file.read_line()
    //         );

        

    //     stop = high_resolution_clock::now();
    //     auto duration = duration_cast<microseconds>(stop - start);
    //     printf("Execution time: %u\n", duration.count());
    // }


    return 0;
}
