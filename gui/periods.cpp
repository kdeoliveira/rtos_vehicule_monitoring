#include "periods.h"

periods::periods(QObject *parent)
    : QObject{parent}, m_input_buffer{"m_buffer_input"}
{
    this->m_periodEngineCoolant = 5;
    this->m_periodRpm = 5;
    this->m_periodSpeed = 5;
    this->m_periodFuel = 5;
    this->m_periodGear = 5;

    for(int i{0}; i < 54; i++){
        //Init all periods to default 5
        this->m_input_buffer->periods[i] = 5;
    }
}
