#ifndef INPUTDATA_H
#define INPUTDATA_H

#include <QObject>

#include <consumer.h>
#include <QVariantList>

#include "reader.h"

class InputData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(float rpm READ rpm WRITE setRpm NOTIFY rpmChanged)
    Q_PROPERTY(int engine_coolant READ engine_coolant WRITE setEngineCoolant NOTIFY engine_coolantChanged)
    Q_PROPERTY(float fuel_consumption READ fuel_consumption WRITE setFuel_consumption NOTIFY fuel_consumptionChanged)
    Q_PROPERTY(int gear READ gear WRITE setGear NOTIFY gearChanged)
    Q_PROPERTY(bool bufferStatus READ bufferStatus WRITE setBufferStatus NOTIFY bufferStatusChanged)
    Q_PROPERTY(int dial READ dial WRITE setDial NOTIFY dialChanged)




public:
    explicit InputData(QObject *parent = nullptr);
    ~InputData(){
        delete m_thread;
    }

    float speed(){
        return this->m_speed;
    }

    float rpm(){
        return this->m_rpm;
    }
    int engine_coolant(){
        return this->m_engine_coolant;
    }
    float fuel_consumption(){
        return this->m_fuel_consumtpion;
    }
    int gear(){
        return this->m_gear;
    }
    bool bufferStatus(){
        return this->m_bufferStatus;
    }
    int dial(){
        return this->m_dial;
    }



    void init(const char*);


    void setSpeed(const float& speed){
        if(this->m_speed == speed){
            return;
        }

        m_speed= speed;
        emit speedChanged(m_speed);
    }

    void setRpm(const float& rpm){
        if(this->m_rpm== rpm){
            return;
        }

        m_rpm= rpm;
        emit rpmChanged(m_speed);
    }

    void setEngineCoolant(const int& engine){
        if(this->m_engine_coolant== engine){
            return;
        }

        m_engine_coolant= engine;
        emit engine_coolantChanged(engine);
    }

    void setGear(const int& gear){
        if(this->m_gear== gear){
            return;
        }

        m_gear= gear;
        emit gearChanged(gear);
    }

    void setFuel_consumption(const float& fuel){
        if(this->m_fuel_consumtpion== fuel){
            return;
        }

        m_fuel_consumtpion= fuel;
        emit fuel_consumptionChanged(fuel);
    }

    void setBufferStatus(const bool& status){
        if(this->m_bufferStatus== status){
            return;
        }

        m_bufferStatus= status;
        emit bufferStatusChanged(status);
    }
    void setDial(const int& dial){

        if(this->m_dial== dial){
            return;
        }

        int temp_sec = dial / 1000;
        int temp_nsec = (dial % 1000) * rtos::Timer::MILLION;

        m_shared_mem_timer->current_val_seconds = temp_sec;
        m_shared_mem_timer->current_val_nanoseconds = temp_nsec;




        m_dial = dial;
        emit dialChanged(dial);
    }


signals:
    void speedChanged(float);
    void rpmChanged(float);
    void engine_coolantChanged(int);
    void fuel_consumptionChanged(float);
    void gearChanged(int);
    void bufferStatusChanged(bool);
    void dialChanged(int);


private:
    float m_speed;
    float m_rpm;
    float m_fuel_consumtpion;
    int m_engine_coolant;
    int m_gear;
    int m_dial;
    bool m_bufferStatus;

    QMainThread* m_thread;
    Reader* m_reader;
    rtos::SharedMem<buffer_clock> m_shared_mem_timer;
//    rtos::SharedMem<buffer_packets> m_shared_input_buffer;


public:
    pthread_t get_thread_id(){
        return this->m_thread->get_thread_id();
    }

public slots:
    void readPipe(const float&, const float&, const int&, const float&, const int&);
    void getStatus(const bool&);


};

#endif // INPUTDATA_H
