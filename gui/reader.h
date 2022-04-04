#ifndef READER_H
#define READER_H

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

#include "../defintion.hpp"

#include <consumer.h>

//struct _buffer_clock{
//    int current_val_seconds = 1;
//    int current_val_nanoseconds = 0;
//};

//typedef struct _buffer_clock buffer_clock;

class Reader : public QObject, public rtos::Task<char *>
{
    Q_OBJECT


public:
    explicit Reader(const char* shared_name, QObject *parent) : QObject(parent), m_input_buffer{shared_name}
    {
        m_input_buffer->semaphore_access = sem_open("/sem_access", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1);

        this->m_abort = false;

    }


    virtual ~Reader(){
        mutex.lock();
        m_abort = true;
        sem_close(m_input_buffer->semaphore_access);
        sem_close(m_input_buffer->semaphore_modification);
        mutex.unlock();
    }

    void stop(){
        mutex.lock();
        m_abort = true;
        mutex.unlock();
    }

signals:
    void bufferRead(const float&, const float&, const int&, const float&, const int&);
    void currentStatus(const bool&);

protected:
    void run(){
//        char* rcv;

//        while( read(this->m_file_descriptor, &rcv, sizeof(float)) > 0 || m_abort != false){
//            float temp = (float) atof(rcv);
//            emit bufferRead(temp);
//        }



//        while(true){
//            for(int i {0} ; i < 54; i++ ){
//                std::cout << this->m_input_buffer->buffer[i];

//            }



            if(this->m_input_buffer->status != 1){
                emit currentStatus(false);
            }else{
                emit currentStatus(true);
            }


//            std::cout << std::endl;
//            qDebug() << this->m_input_buffer->buffer[SensorsHeader::Vehicle_speed].payload;
//            std::cout << (int) this->m_input_buffer->buffer[SensorsHeader::Current_Gear].payload << std::endl;
            emit bufferRead(this->m_input_buffer->buffer[SensorsHeader::Vehicle_speed].payload, this->m_input_buffer->buffer[SensorsHeader::Engine_speed].payload, (int) this->m_input_buffer->buffer[SensorsHeader::Engine_coolant_temperature].payload, this->m_input_buffer->buffer[SensorsHeader::Fuel_consumption].payload, (int) this->m_input_buffer->buffer[SensorsHeader::Current_Gear].payload );

//            sleep(1);


//        }
    }

private:
    bool m_abort;
    QMutex mutex;
    float m_value = 0.0;
    rtos::SharedMem<buffer_packets> m_input_buffer;


};

#endif // READER_H
