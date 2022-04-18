#include "inputdata.h"
#include "QDebug"

InputData::InputData(QObject *parent) : QObject(parent), m_shared_mem_timer{"m_input_buffer_clock"}
{
    this->m_speed = 0.00;
    this->m_dial = 1000;

    m_shared_mem_timer->current_val_seconds = 1;
    m_shared_mem_timer->current_val_nanoseconds = 0;

}




void InputData::init(const char* name){
    m_reader = new Reader(name, this);
    m_thread = new QMainThread(1, m_reader);
    connect(m_reader, &Reader::bufferRead, this, &InputData::readPipe);
    connect(m_reader, &Reader::currentStatus, this, &InputData::getStatus);
}


void InputData::readPipe(const float& speed, const float& rpm, const int& engine, const float& fuel, const int& gear){
    setSpeed(speed);
    setRpm(rpm);
    setEngineCoolant(engine);
    setFuel_consumption(fuel);
    setGear(gear);
}

void InputData::getStatus(const bool & status)
{

    setBufferStatus(status);
}
