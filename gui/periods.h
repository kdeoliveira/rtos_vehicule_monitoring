#ifndef PERIODS_H
#define PERIODS_H

#include <rtos_shared_mem.hpp>
#include <../src/common.hpp>
#include <QObject>
#include <QDebug>

class periods : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int periodFuel READ periodFuel WRITE setPeriodFuel NOTIFY periodFuelChanged)
    Q_PROPERTY(int periodSpeed READ periodSpeed WRITE setPeriodSpeed NOTIFY periodSpeedChanged)
    Q_PROPERTY(int periodRpm READ periodRpm WRITE setPeriodRpm NOTIFY periodRpmChanged)
    Q_PROPERTY(int periodEngineCoolant READ periodEngineCoolant WRITE setPeriodEngineCoolant NOTIFY periodEngineCoolantChanged)
    Q_PROPERTY(int periodGear READ periodGear WRITE setPeriodGear NOTIFY periodGearChanged)


public:
    explicit periods(QObject *parent = nullptr);


    int periodFuel(){
        return m_periodFuel;
    }
    int periodGear(){
        return m_periodGear;
    }
    int periodRpm(){
        return m_periodRpm;
    }
    int periodEngineCoolant(){
        return m_periodEngineCoolant;
    }
    int periodSpeed(){
        return m_periodSpeed;
    }

    void setPeriodFuel(int x){
        if(this->m_periodFuel == x) return;
        m_periodFuel = x;
        this->m_input_buffer->periods[SensorsHeader::Fuel_consumption] = x;
        emit periodFuelChanged(x);
    }
    void setPeriodSpeed(int x){
        if(this->m_periodSpeed == x) return;
        m_periodSpeed = x;
        this->m_input_buffer->periods[SensorsHeader::Vehicle_speed] = x;
        emit periodSpeedChanged(x);
    }
    void setPeriodEngineCoolant(int x){
        if(this->m_periodEngineCoolant == x) return;
        m_periodEngineCoolant = x;
        this->m_input_buffer->periods[SensorsHeader::Engine_coolant_temperature] =x ;
        emit periodEngineCoolantChanged(x);
    }
    void setPeriodRpm(int x){
        if(this->m_periodRpm == x) return;
        m_periodRpm = x;
        this->m_input_buffer->periods[SensorsHeader::Engine_speed] = x;
        emit periodRpmChanged(x);
    }
    void setPeriodGear(int x){
        if(this->m_periodGear== x) return;
        m_periodGear= x;
        this->m_input_buffer->periods[SensorsHeader::Current_Gear] = x;

        emit periodGearChanged(x);
    }

signals:
    void periodFuelChanged(int);
    void periodSpeedChanged(int);
    void periodRpmChanged(int);
    void periodEngineCoolantChanged(int);
    void periodGearChanged(int);



private:
    int m_periodFuel;
    int m_periodSpeed;
    int m_periodRpm;
    int m_periodEngineCoolant;
    int m_periodGear;
    rtos::SharedMem<buffer_packets> m_input_buffer;
};

#endif // PERIODS_H
