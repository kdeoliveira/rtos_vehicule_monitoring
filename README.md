<p align="center"> 
  <img src="https://www.qnx.com/style-v2/img/bb-qnx-logo.png" alt="qnx"/>
  <img src="https://www.qt.io/hubfs/qt-design-system/assets/logos/qt-logo.svg" width="100" alt="qt">
</p>


## Real Time Application of Vehicule Monitoring System
This project involves all or part of the design, implementation, testing, and analysis of a simplified
real-time system for monitoring of vehicle’s health conditions.

### Overview

- The application consist of two parts, a producer which gathers information a dataset and a consumer that displays those information to the user
- The project is divided into two sections: src and gui both implemented separately
- The producer is built with CMake and makes usage of the QNX SDP 7.0
- The consumer is build with qmake and uses QNX SDP 7.0 and Qt 5.12.2 framework
- A toolchain file is provided so the code can be either compiled to any Unix system or cross-compiled to any supported QNX x64 system
- Note that in order to built the GUI using GCC, the proper qmake tool needs to be generated from the Qt source files.

### Requirements
- Linux x86_x64 OS
- C/C++ std libraries
- QNX SDP 7.0
- Qt 5.12.2 (at least its libraries)
- CMake 3.X

### Build on Linux
You may compile this application for a Linux host machine:  

Build with CMake:  
``cmake -B $PREFIX_DIR -S . -G Ninja -DCMAKE_BUILD_TYPE=Release``  

### Build for QNX 
This application can be cross compiled using one of the two ways below:  

Run the shell script:  
``sh built_to_qnx.sh $IP_ADDR_REMOTE``  


Build with CMake:  
``cmake -B $PREFIX_DIR -S . -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=qnx.nto.toolchain2.cmake``  

### Build GUI
The user interface is built with Qt. Therefore, it can be compiled directly from Qt Creator using the following kit settings:
- sysroot: $QNX_HOST/target/qnx7/x86_64
- C: qcc compiler path
- C++: q++ compiler path
- Debugger: ntox86_64-gdb
- Qt version: 5.12.2

### Running on QNX VM 
Before execution, all required libraries must be loaded and linked on the target machine
- All built Qt libraries must be deployed to the target machine
- Export all required envrionment variables (refer to Qt for QNX on Qt website)
- Upload required shared libraries (see lib/rtos_common/CmakeLists.txt)
- Initiate the Screen graphics subsystem on the target machine

With the target machine properly set, the application can be deployed and executed. It is recommended to use the default VM image provided by the QNX Software Center, as it comes with all the required utilities and services.
If you are building an image from a BSP file, ensure that all services [required by Qt]("https://doc.qt.io/qt-5/qnx.html#qnx-screen-graphics-subsystem") are running before executing this application.

For more information on how to build a QNX OS image, access QNX user's guide