

## Wiki - Implementation of Vehicule Monitoring System

This project consists in the implementation of a real time application which emulates monitoring system that periodically obtains information of vehicule conditions. Such systems usually gather information from the set of sensors installed in the vehicule and make use of a Controller Area Network serial interface for communication. Since the purpose of this application is only to emulate such behavior, a dataset file is used instead to replicate the behavior of such serial communication.  

Moreover, the architecture of this application requires the implementation a Producer-consumer pattern, a classic concurrent design pattern, which two parallel tasks are required to run in parallel such that one produces a defined set of data and the other consumes that data.

As previously mentionned, this application is built for x86 and x64 QNX Neutrino systems which implements the standard POSIX features and libraries.

- libpthread
- librt

Note that since the QNX SDP already defines most of the extensions provided by the above libraries, no dynamic linkage is required.

### Internal Libraries

In order to facilitate the implementation of the main system, each feature provided by the POSIX API has been implemented in a separate module which later is statically bounded to the main application.

- librtos_common.so: It contains the implementation of callback and predicate types through a single generic interface (type erasure idiom)
- librtos_data.a: Implementation of sequence containers that stores common elements used by this system
- librtos_ipc.a: Definition and implementation of realtime, threads, filesystems and IPC functionalities provided by the POSIX API

All the above libraries are defined inside the `rtos::` namespace in order to prevent name collision with other external libraries used in this project.  

### Executables

The main application consists of 3 executables, all built on top of the above libaries:

- process manager: It is responsible for managing the creation and termination of each process, and creation of a interval timer
- producer:  Periodically generates data from the dataset.csv file
- consumer.cpp: Periodically outputs the data produced

Each of those application run in different process and do not share memory spaces. Therefore, in order to share data between those processes, it is required to use one of the inter-process communication method defined in the `librtos_ipc.a` library.  

### Gui

An alternative implemenation is provided for the consumer, which uses a GUI where the user can interact. Such user-interface is built with the help of the Qt framework, which provides direct support for QNX Neutrino RTOS systems.