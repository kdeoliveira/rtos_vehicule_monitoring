cmake_minimum_required(VERSION 3.0.0)
project(rtos_vehicule_monitoring VERSION 0.1.0)

add_executable(${PROJECT_NAME} main.cpp)

if(DEFINED QNX_PROCESSOR)
    add_compile_definitions(_QNX_x86_64)
    add_compile_definitions(DATASET_PATH="/opt/apps/dataset.csv")
else()
    add_compile_definitions(DATASET_PATH="/home/dev/dev/rtos_vehicule_monitoring/data/dataset.csv")
endif()



include(CTest)
enable_testing()

add_subdirectory(lib)
add_subdirectory(src)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(
    ${PROJECT_NAME} rtos_ipc
)