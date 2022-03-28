<p align="center"> 
  <img src="https://www.qnx.com/style-v2/img/bb-qnx-logo.png" alt="qnx"/>
  <img src="https://www.qt.io/hubfs/qt-design-system/assets/logos/qt-logo.svg" width="100" alt="qt">
</p>


## Real Time Application of Vehicule Monitoring System
This project involves all or part of the design, implementation, testing, and analysis of a simplified
real-time system for monitoring of vehicle’s health conditions.

### Project

- The application consist of two parts, a producer which gathers information a dataset
- and a consumer that displays those information to the user
- The project is built with CMake (producer) and qmake (consumer) and uses QNX SDP 7.0 and Qt 5.12.2 framework.
- A toolchain file is provided so the code can be either compiled to any Unix system or cross-compiled to any supported QNX x64 system
- Note that in order to built the GUI using GCC, the proper qmake tool needs to be generated from the Qt source files.

### Build
This application must be built from a Linux x86_64 system  
To compile either run the shell script:  
``sh built_to_qnx.sh $IP_ADDR_REMOTE``  

Or with CMake:  
``cmake -B $PREFIX_DIR -S . -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=qnx.nto.toolchain2.cmake``  
