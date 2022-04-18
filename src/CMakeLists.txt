

add_executable(producer producer.cpp)

add_executable(consumer consumer.cpp)



target_link_libraries(
    consumer rtos_data
)

target_link_libraries(
    consumer rtos_ipc
)

target_link_libraries(
    producer rtos_ipc
)

target_link_libraries(
    producer rtos_data
)

