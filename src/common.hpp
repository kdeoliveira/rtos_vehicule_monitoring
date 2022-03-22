#pragma once
#include <map>
#include <ostream>

struct period_task
{
    period_task(){}
    pthread_t thread_id = 0;
    u_int8_t period = 0;
};




enum SensorsHeader : u_int8_t{
    
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



std::ostream& operator << (std::ostream& os, const SensorsHeader& data){
    switch(data){
        case SensorsHeader::Fuel_consumption:
            return os << "Fuel Consumption";
        case SensorsHeader::Accelerator_Pedal_value:
            return os << "Accelerator Pedal Value";
        case SensorsHeader::Throttle_position_signal:
            return os << "Throttle Position Signal";
        case SensorsHeader::Intake_air_pressure:
            return os << "Intake Air Pressure";
        case SensorsHeader::Filtered_Accelerator_Pedal_value:
            return os << "Filtered Accelerator Pedal";
        case SensorsHeader::Absolute_throttle_position:
           return  os << "Absolute Throttle Position";
        case SensorsHeader::Engine_soacking_time:
           return  os << "Engine Soacking Time";
        case SensorsHeader::Inhibition_of_engine_fuel_cut_off:
            return os << "Inhibition of Engine Fuel Cut-Off";
        case SensorsHeader::Engine_in_fuel_cut_off:
            return os << "Engine in Fuel Cut-Off";
        case SensorsHeader::Fuel_Pressure:
            return os << "Fuel Pressure";
        case SensorsHeader::Long_Term_Fuel_Trim_Bank1:
            return os << "Long Term Fuel Trim Bank";
        default:
            return os << "Unknwon";

    }
}

std::map<std::string, SensorsHeader> map_sensors {
    {"Fuel_consumption", SensorsHeader::Fuel_consumption},
    {"Accelerator_Pedal_value", SensorsHeader::Accelerator_Pedal_value},
    {"Throttle_position_signal", SensorsHeader::Throttle_position_signal},
    {"Short_Term_Fuel_Trim_Bank1", SensorsHeader::Short_Term_Fuel_Trim_Bank1},
    {"Intake_air_pressure", SensorsHeader::Intake_air_pressure},
    {"Filtered_Accelerator_Pedal_value", SensorsHeader::Filtered_Accelerator_Pedal_value},
    {"Absolute_throttle_position", SensorsHeader::Absolute_throttle_position},
    {"Engine_soacking_time", SensorsHeader::Engine_soacking_time},
    {"Inhibition_of_engine_fuel_cut_off", SensorsHeader::Inhibition_of_engine_fuel_cut_off},
    {"Engine_in_fuel_cut_off", SensorsHeader::Engine_in_fuel_cut_off},
    {"Fuel_Pressure", SensorsHeader::Fuel_Pressure},
    {"Long_Term_Fuel_Trim_Bank1", SensorsHeader::Long_Term_Fuel_Trim_Bank1},
};

union SensorData{
    float val_n;
    char val_c;

            friend std::ostream& operator << (std::ostream& os, const SensorData& data){
                if(sizeof(data) == sizeof(float))
                // if(atof(&data.val_c) == 0.0)
                    os << data.val_c;
                else
                    os << data.val_n;

                

                return os;
            }
};

typedef float SensorValue;

template<typename T, typename K>
struct or_type{
    or_type() : val_t{new T}, val_k{new K}{}
    
    or_type(T&& x) : val_t{std::forward<T>(x)}, val_k{nullptr}{}
    
    or_type(K&& x) : val_k{std::forward<K>(x)}, val_t{nullptr}{}

    void operator()(const T& data){
        val_t = data;
    }
    void operator()(const K& data){
        val_k = data;
    }

    friend std::ostream& operator << (std::ostream& os, const or_type<T, K>& data){
        if(data.val_k != nullptr)
            os << *data.val_k;
        else if(data.val_t != nullptr)
            os << * data.val_t;

        return os;
    }

    T* val_t;
    K* val_k;
};


// BUFFERING IN/OUT PACKETS PLUS HEADERS

#include <rtos_packet.hpp>
#include <rtos_buffer.hpp>
#include <semaphore.h>


 struct buffer_packet{
    char temp_buffer[30];
    int status;
    rtos::packet_data<SensorsHeader, SensorValue> buffer[54];
    sem_t* semaphore_access;
    sem_t* semaphore_modification;
 };

 typedef struct buffer_packet buffer_packets;




#include <chrono>

using namespace std::chrono;