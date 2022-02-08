

add_executable(writer writer.cpp)
add_executable(reader reader.cpp)
add_executable(producer producer.cpp)





target_link_libraries(
    writer rtos_parser
)

target_link_libraries(
    writer rtos_ipc
)
target_link_libraries(
    writer pthread
)

target_link_libraries(
    reader rtos_ipc
)

target_link_libraries(
    reader rtos_parser
)

target_link_libraries(
    producer rtos_ipc
)