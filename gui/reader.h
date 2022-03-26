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

enum SensorsHeader : unsigned char{

    Fuel_consumption,
    Accelerator_Pedal_value,
    Throttle_position_signal,
    Short_Term_Fuel_Trim_Bank1,
    Intake_air_pressure,
    Filtered_Accelerator_Pedal_value,
    Absolute_throttle_position,
    Engine_soacking_time,
    Inhibition_of_engine_fuel_cut_off,
    Engine_in_fuel_cut_off,
    Fuel_Pressure,
    Long_Term_Fuel_Trim_Bank1
};



 struct buffer_packet{
    char temp_buffer[30];
    int status;
    rtos::packet_data<SensorsHeader, float> buffer[54];
    sem_t* semaphore_access;
    sem_t* semaphore_modification;
 };

 typedef struct buffer_packet buffer_packets;


class Reader : public QThread
{
    Q_OBJECT


public:
    explicit Reader(const char* shared_name, QObject *parent) : QThread(parent), m_input_buffer{shared_name}
    {
        m_input_buffer->semaphore_access = sem_open("/sem_access", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1);
        this->m_abort = false;
        start();
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
    void bufferRead(const float&);

protected:
    void run(){
//        char* rcv;



//        while( read(this->m_file_descriptor, &rcv, sizeof(float)) > 0 || m_abort != false){
//            float temp = (float) atof(rcv);
//            emit bufferRead(temp);
//        }

        while(true){
            qDebug() << this->m_input_buffer->buffer[SensorsHeader::Accelerator_Pedal_value].payload;
            emit bufferRead(this->m_input_buffer->buffer[0].payload);
            sleep(1);
        }
    }

private:
    bool m_abort;
    QMutex mutex;
    float m_value = 0.0;
    rtos::SharedMem<buffer_packets> m_input_buffer;


};

#endif // READER_H
