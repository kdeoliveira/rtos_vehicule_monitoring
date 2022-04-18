#pragma once
#include <map>
#include <ostream>
#include <rtos_scheduler.hpp>
#include <rtos_task.hpp>
#include <rtos_algorithm.hpp>
#include <rtos_packet.hpp>
#include <rtos_buffer.hpp>
#include <semaphore.h>

/**
 * @brief Structure used by the scheduler
 *
 */
struct period_task
{
    period_task(){}
    pthread_t thread_id = 0;
    u_int8_t period = 0;
};



/**
 * @brief Enumaration of columns provided by the dataset.csv
 *
 */
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
    Long_Term_Fuel_Trim_Bank1,
    Engine_speed,
    Engine_torque_after_correction,
    Torque_of_friction,
    Flywheel_torque_after_torque_interventions,
    Current_spark_timing,
    Engine_coolant_temperature,
    Engine_Idel_Target_Speed,
    Engine_torque,
    Calculated_LOAD_value,
    Minimum_indicated_engine_torque,
    Maximum_indicated_engine_torque,
    Flywheel_torque,
    Torque_scaling_factor_standardization,
    Standard_Torque_Ratio,
    Requested_spark_retard_angle_from_TCU,
    TCU_requests_engine_torque_limit_ETL,
    TCU_requested_engine_RPM_increase,
    Target_engine_speed_used_in_lock_up_module,
    Glow_plug_control_request,
    Activation_of_Air_compressor,
    Torque_converter_speed,
    Current_Gear,
    Engine_coolant_temperature_2,
    Wheel_velocity_front_left_hand,
    Wheel_velocity_rear_right_hand,
    Wheel_velocity_front_right_hand,
    Wheel_velocity_rear_left_hand,
    Torque_converter_turbine_speed_Unfiltered,
    Clutch_operation_acknowledge,
    Converter_clutch,
    Gear_Selection,
    Vehicle_speed,
    Acceleration_speed_Longitudinal,
    Indication_of_brake_switch_ON_OFF,
    Master_cylinder_pressure,
    Calculated_road_gradient,
    Acceleration_speed_Lateral,
    Steering_wheel_speed,
    Steering_wheel_angle,
    Time_s_,
    Class,
    PathOrder
};



inline std::ostream& operator << (std::ostream& os, const SensorsHeader& data){
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

/**
 * @brief Thread class implementation that makes use of a runnable task
 *
 */
class MainThread : public rtos::Thread<char*>{
    public:
        MainThread(int id, rtos::Task<char *>* task) : m_id{id}, rtos::Thread<char *>{task, false, SIGUSR2}{    //Sensible to SIGUSR2 signal
        }

    private:
        int m_id;
};


/**
 * @brief Scheduler Algorithm implementation used by both producer and consumer processes
 *
 */
class SchedulerAlgo : public rtos::algorithm<period_task>
{
public:
    //TODO:
    //Note that this size should be implemented internally as it may result on crash
    SchedulerAlgo(const int _sz, const char* name) : rtos::algorithm<period_task>{_sz} , name_of_scheduler{name}{}

    template<typename T = period_task>
    void push(T& task)
    {
        this->m_queue[this->m_index++] = task;
    }

    template<typename T = period_task , typename...K>
    void push(T& task, K&...k)
    {
        this->push(k...);
        this->push(task);
    }

    /**
     * @brief Main scheduler used by the processes
     * It a simple clock-driven scheduler where each task is dispatched at a given period
     * Order of priority is not completed and solely based on the position of the task in the m_queue array
     * At each clock cycle of the timer, the Scheduler will verify if any task is to be disaptched by perofrming a simple search on the m_queue array.
     * @code (_timer_cycle->cycles + 1) % this->m_queue[i].period == 0
     * If a task is found, a pthread_kill sends a signal to that thread so it can be start executing
     * 
     * @param _timer_cycle Timer cycle
     * @param _signmum Signal to be send to threads
     * @return void* return value (optional)
     */
    void *run(rtos::timer_cycle* _timer_cycle, const int &_signmum) const override
    {

        #ifdef DEBUG
            printf("[%s] Cycle: %u\n",name_of_scheduler,  _timer_cycle->cycles);
        #endif

            //kill thread by period value provided
            for(int i{0}; i < this->size() ; i++){


                if((_timer_cycle->cycles + 1) % this->m_queue[i].period == 0){

                    #ifdef DEBUG
                        printf("[debug - %s] period of task %u -> %u \n", name_of_scheduler, _timer_cycle->cycles, this->m_queue[i].period);
                        printf("[%s] ptask id: %lu\n", name_of_scheduler, this->m_queue[i].thread_id);
                    #endif

                    pthread_kill(this->m_queue[i].thread_id, _signmum);

                    return nullptr;
                }
            }
        return nullptr;
    }

    private:
        const char* name_of_scheduler;
};



/**
 * @brief Buffer used by the producer and consumer
 *
 */
 struct buffer_packet{
    char temp_buffer[30];
    int status;
    rtos::packet_data<SensorsHeader, SensorValue> buffer[54];
    sem_t* semaphore_access;
    sem_t* semaphore_modification;
    //For both consumer and producer accces, this should be modified to regular array and initialized in consumer
    int periods[54];
 };

 typedef struct buffer_packet buffer_packets;

