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


#include <chrono>

using namespace std::chrono;