#ifndef SCHEDULER_H
#define SCHEDULER_H


#include <QObject>

#include <QThread>
#include <QMutex>

#include <sys/types.h>
#include <unistd.h>
#include <QDebug>
#include <rtos_common.hpp>
#include "rtos_shared_mem.hpp"
#include <rtos_packet.hpp>
#include <rtos_buffer.hpp>
#include <semaphore.h>


#include <iostream>

#include <../src/common.hpp>


#include <consumer.h>

class SchedulerThread : public QThread
{



public:
    SchedulerThread() = default;

    explicit SchedulerThread(const pthread_t thread_id, QObject *parent) : QThread(parent)
    {
        algo = new ConsumerSchedulerAlgo{1};

        sched_consumer = new rtos::Scheduler<period_task>(SIGUSR1, algo, 5);

        period_task c_task;
        c_task.period = (uint8_t)5;
        c_task.thread_id = thread_id;

        algo->push(c_task);

        this->m_abort = false;
        this->start();
    }


    virtual ~SchedulerThread(){

        mutex.lock();
        m_abort = true;
        mutex.unlock();
    }

    void stop(){
        mutex.lock();
        m_abort = true;
        delete sched_consumer;
        mutex.unlock();
    }



protected:
    void run(){

        sched_consumer->dispatch(SIGUSR2);
    }

private:
    bool m_abort;
    QMutex mutex;
    rtos::Scheduler<period_task>* sched_consumer;
    ConsumerSchedulerAlgo* algo;

};


#endif // SCHEDULER_H
