
project(rtos_common)

include_directories(
    ./include
    ./src
)

add_library(${PROJECT_NAME} SHARED ./src/rtos_common.cpp)


target_include_directories(
    ${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
