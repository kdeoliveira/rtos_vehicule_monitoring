
project(rtos_ipc)

include_directories(
    ./include
    ./src
)

add_library(${PROJECT_NAME} INTERFACE)


target_include_directories(
    ${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(
    rtos_ipc INTERFACE pthread
)

target_link_libraries(
    rtos_ipc INTERFACE rtos_common
)

target_link_libraries(
    ${PROJECT_NAME} INTERFACE rt
)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)