#include "inputdata.h"
#include "QDebug"

InputData::InputData(QObject *parent) : QObject(parent)
{
    this->m_speed = 0.00;
}




void InputData::init(const char* name){
    m_thread = new Reader(name, this);

    connect(m_thread, &Reader::bufferRead, this, &InputData::readPipe);
}


void InputData::readPipe(const float& input){

    setSpeed(input);
}
