#pragma once
#include <map>


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


#include <chrono>

using namespace std::chrono;