
project(rtos_data)

include_directories(
    ./include
    ./src
)

add_library(${PROJECT_NAME} INTERFACE)


target_include_directories(
    ${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(
    ${PROJECT_NAME} INTERFACE rtos_common
)
