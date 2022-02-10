#include <rtos_scheduler.hpp>
#include <rtos_algorithm.hpp>
#include <rtos_timer.hpp>

#include <sys/types.h>
#include <sys/wait.h>

struct period_task
{
    pthread_t thread_id;
    uint8_t period;
};

class ProducerSchedulerAlgo : public rtos::algorithm<period_task>
{
public:
    ProducerSchedulerAlgo(const int _sz) : rtos::algorithm<period_task>{_sz} {}

    void push(period_task task)
    {
        this->m_queue[this->m_index++] = task;
    }

    void *run(rtos::timer_cycle *_timer_cycle, const int &_signmum) const override
    {
        // if(this->m_index == 0) return nullptr;

        static int start;
        static u_int8_t cycles = 0;
        int current;
        struct timespec tv;

        if (start == 0)
        {
            clock_gettime(CLOCK_MONOTONIC, &tv);
            start = tv.tv_sec * rtos::Timer::THOUSAND + tv.tv_nsec / rtos::Timer::MILLION;
        }

        clock_gettime(CLOCK_MONOTONIC, &tv);
        current = tv.tv_sec * rtos::Timer::THOUSAND + tv.tv_nsec / rtos::Timer::MILLION;

        if (cycles > 0)
        {
            printf("Current time spend: %f\n",  (double)(current - start));
        }

        ++cycles;

        return nullptr;
    }
};

int main(int argc, char *argv[])
{

    // siginfo_t info;
    // sigset_t set;
    // sigemptyset(&set);
    // sigaddset(&set, SIGUSR1);
    // sigprocmask(SIG_BLOCK, &set, nullptr);

    pid_t pid = fork();

    if (pid < 0)
        return -1;

    if (pid == 0)
    {

        rtos::Timer m_timer{CLOCK_REALTIME, SIGUSR1, 5};

        if (m_timer.start(2, 0) < 0)
            perror("timer_settime");

        m_timer.onNotify([&](void *val)
                         {
            kill(getppid(), SIGUSR1);
            puts("HERE"); });

        m_timer.notify(0, SIGUSR1, nullptr);
    }
    else
    {
        auto *algo = new ProducerSchedulerAlgo{2};
        rtos::Scheduler sched{SIGUSR1, algo};

        //TOOD: !!Check if sigwaitinfo() indeed stop process!!
        sched.dispatch();
    }


    return EXIT_SUCCESS;
}