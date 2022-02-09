#include <cstdlib>
#include <rtos_task.hpp>
#include <rtos_timer.hpp>
class FuelConsumption : public rtos::Task<char *>{
    public:
        char* run() override{
            int i = 0;

            while(i < 20){
                printf("Current index: %d\n", i);
                i++;
            }
            return nullptr;
        }

        ~FuelConsumption(){

        }
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
        FuelConsumption* fc = new FuelConsumption();
        std::unique_ptr<MainThread> thread = std::make_unique<MainThread>(5, fc);

        thread->start();
        thread->join();
    }catch(const char* e){
        puts(e);
    }


    rtos::Timer m_timer(CLOCK_REALTIME, SIGUSR1, 5);

    if(m_timer.start(2,0) < 0) perror("timer_settime");

    m_timer.onNotify([](void* val){
        static int start;
        static u_int8_t cycles = 0;
        int current;
	    struct timespec tv;
        


        if(start == 0){
            clock_gettime(CLOCK_MONOTONIC, &tv);
            start = tv.tv_sec * rtos::Timer::THOUSAND + tv.tv_nsec / rtos::Timer::MILLION;
        }

        clock_gettime(CLOCK_MONOTONIC, &tv);
        current = tv.tv_sec * rtos::Timer::THOUSAND + tv.tv_nsec / rtos::Timer::MILLION;

        if(cycles > 0){
            printf("Current cycle: %u and Time spend: %f\n", cycles, (double)(current - start)/cycles);
        }
        
        ++cycles;
        

        
    });
    // sigset_t sigst;
    // sigemptyset(&sigst); // initialize a signal set
	// sigaddset(&sigst, SIGUSR1); // add SIGALRM to the signal set
	// sigprocmask(SIG_BLOCK, &sigst, NULL); //block the signal

    // while(true){
    //     sigwait(&sigst, nullptr);
    //     puts("HERE");
    // }

    m_timer.notify(0, SIGUSR1, nullptr);


    return EXIT_SUCCESS;
}