#include <cstdlib>
#include <rtos_task.hpp>
#include <rtos_timer.hpp>
#include <rtos_packet.hpp>
#include "common.hpp"
#include <stack>
#include <mutex>
#include <condition_variable>
#include <rtos_buffer.hpp>

std::condition_variable m_cd;
bool ready = true;

class FuelConsumption : public rtos::Task<char *>{
    public:
        FuelConsumption(std::stack<char *> x) : rtos::Task<char *>{x}{}
        void run() override{
            int i = 0;

            while(true){

                std::unique_lock<std::mutex> ul(this->m_mx);

                m_cd.wait(ul, [&]{
                    return !this->m_input_stack.empty();
                });

                
                std::cout << this->m_input_stack.top() << std::endl;
                this->m_input_stack.pop();
                ready = true;
                m_cd.notify_one();
            }
            
        }

        ~FuelConsumption(){
            // delete m_packet_data;
            // delete m_packet_header;
        }

    private:
        std::mutex m_mx;

        
};

class Producer : public rtos::Task<char *>{
    public:
    Producer(std::stack<char *> x) : rtos::Task<char *>{x}{}
    void run() override{
        int i = 0;
        while(true){
            std::cout << "Pushing to queue" << std::endl;

            std::unique_lock<std::mutex> ul(this->m_mx);

            m_cd.wait(ul, [&]{
            return ready;
            });
            ready = false;

            char * temp = new char[20];
            sprintf(temp, "test: %d", i);

            this->m_input_stack.push(temp);
            i++;
            
            m_cd.notify_one();
        }
    }
    private:
        std::mutex m_mx;


};

class MainThread : public rtos::Thread<char*>{
    public:
        MainThread(int id, rtos::Task<char *>* task) : m_id{id}, rtos::Thread<char *>{task, false}{
        }

    private:
        int m_id;
};

int main(int argc, char *argv[])
{

    try{
        std::stack<char *> producer_queue;

        rtos::Task<char*>* fc = new FuelConsumption(producer_queue);
        rtos::Task<char*>* pd = new Producer(producer_queue);

        std::unique_ptr<MainThread> thread = std::make_unique<MainThread>(5, fc);
        std::unique_ptr<MainThread> thread1 = std::make_unique<MainThread>(10, pd);

        rtos::buffer<char *> buff(2);

        buff[0] = "Kevin";
        buff[1] = "De Oliveira";

        std::cout << buff[0] << std::endl;

        buff.pop(0);
        

        for(auto& x : buff){
            if(x != nullptr)
                puts(x);
        }



        // thread->start();
        // thread1->start();
        // thread->join();
        // thread1->join();
    }catch(const char* e){
        puts(e);
    }







    return EXIT_SUCCESS;
}