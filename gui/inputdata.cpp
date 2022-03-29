#include "inputdata.h"
#include "QDebug"

InputData::InputData(QObject *parent) : QObject(parent)
{
    this->m_speed = 0.00;
}




void InputData::init(const char* name){
    m_thread = new Reader(name, this);

    connect(m_thread, &Reader::bufferRead, this, &InputData::readPipe);
    connect(m_thread, &Reader::currentStatus, this, &InputData::getStatus);
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
