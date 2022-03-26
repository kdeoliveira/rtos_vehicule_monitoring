#ifndef READER_H
#define READER_H

#include <QObject>

#include <QThread>
#include <QMutex>

#include <sys/types.h>
#include <unistd.h>
#include <QDebug>
#include <rtos_common.hpp>
#include "rtos_shared_mem.hpp"
#include <rtos_packet.hpp>
#include <rtos_buffer.hpp>
#include <semaphore.h>

#include <iostream>

enum SensorsHeader : unsigned char{
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



 struct buffer_packet{
    char temp_buffer[30];
    int status;
    rtos::packet_data<SensorsHeader, float> buffer[54];
    sem_t* semaphore_access;
    sem_t* semaphore_modification;
 };

 typedef struct buffer_packet buffer_packets;


class Reader : public QThread
{
    Q_OBJECT


public:
    explicit Reader(const char* shared_name, QObject *parent) : QThread(parent), m_input_buffer{shared_name}
    {
        m_input_buffer->semaphore_access = sem_open("/sem_access", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1);

        this->m_abort = false;
        start();
    }


    virtual ~Reader(){
        mutex.lock();
        m_abort = true;
        sem_close(m_input_buffer->semaphore_access);
        sem_close(m_input_buffer->semaphore_modification);
        mutex.unlock();
    }

    void stop(){
        mutex.lock();
        m_abort = true;
        mutex.unlock();
    }

signals:
    void bufferRead(const float&, const float&, const int&, const float&, const int&);
    void currentStatus(const bool&);

protected:
    void run(){
//        char* rcv;



//        while( read(this->m_file_descriptor, &rcv, sizeof(float)) > 0 || m_abort != false){
//            float temp = (float) atof(rcv);
//            emit bufferRead(temp);
//        }



        while(true){
//            for(int i {0} ; i < 54; i++ ){
//                std::cout << this->m_input_buffer->buffer[i];

//            }

            if(this->m_input_buffer->status != 1){
                emit currentStatus(false);
            }else{
                emit currentStatus(true);
            }


//            std::cout << std::endl;
//            qDebug() << this->m_input_buffer->buffer[SensorsHeader::Vehicle_speed].payload;
//            std::cout << (int) this->m_input_buffer->buffer[SensorsHeader::Current_Gear].payload << std::endl;
            emit bufferRead(this->m_input_buffer->buffer[SensorsHeader::Vehicle_speed].payload, this->m_input_buffer->buffer[SensorsHeader::Engine_speed].payload, (int) this->m_input_buffer->buffer[SensorsHeader::Engine_coolant_temperature].payload, this->m_input_buffer->buffer[SensorsHeader::Fuel_consumption].payload, (int) this->m_input_buffer->buffer[SensorsHeader::Current_Gear].payload );

//            sleep(1);


        }
    }

private:
    bool m_abort;
    QMutex mutex;
    float m_value = 0.0;
    rtos::SharedMem<buffer_packets> m_input_buffer;


};

#endif // READER_H
