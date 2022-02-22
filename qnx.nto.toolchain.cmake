set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
SET (CMAKE_CXX_COMPILER_WORKS 1)

set(QNX_HOST "/home/kdeoliveira/app/qnx/qnx710/host/linux/x86_64")
set(QNX_TARGET "/home/kdeoliveira/app/qnx/qnx7")

SET(CMAKE_CXX_FLAGS_DEBUG "-std=c++14")
SET(CMAKE_CXX_FLAGS_RELEASE "-std=c++14")

if("$ENV{QNX_HOST}" STREQUAL "")
  message(FATAL_ERROR "QNX_HOST environment variable not found. Please set the variable to your host's build tools")
endif()
if("$ENV{QNX_TARGET}" STREQUAL "")
  message(FATAL_ERROR "QNX_TARGET environment variable not found. Please set the variable to the QNX target location")
endif()

set(QNX_HOST "$ENV{QNX_HOST}")
set(QNX_TARGET "$ENV{QNX_TARGET}")

set(QNX TRUE)
set(CMAKE_SYSTEM_NAME QNX)
set(CMAKE_C_COMPILER ${QNX_HOST}/usr/bin/qcc)
set(CMAKE_CXX_COMPILER ${QNX_HOST}/usr/bin/q++)
set(CMAKE_AR "${QNX_HOST}/usr/bin/ntox86_64-ar${HOST_EXECUTABLE_SUFFIX}" CACHE PATH "archiver")
set(CMAKE_RANLIB "${QNX_HOST}/usr/bin/ntox86_64-ranlib${HOST_EXECUTABLE_SUFFIX}" CACHE PATH "ranlib")
#set(THREADS_PTHREAD_ARG 0)
set(THREADS_PTHREAD_ARG "0"
  CACHE STRING "Result from TRY_RUN" FORCE)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Vgcc_ntox86_64 -g -D__USESRCVERSION -D_QNX_SOURCE -DQNX -DTIXML_USE_STL -DEIGEN_MPL2_ONLY -Wno-deprecated-declarations -Wno-unused-parameter -Wno-unused-variable -Wno-ignored-attributes ${EXTRA_CMAKE_C_FLAGS}" CACHE STRING "c_flags")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Vgcc_ntox86_64 -g -D__USESRCVERSION -D_QNX_SOURCE -DQNX -DTIXML_USE_STL -DEIGEN_MPL2_ONLY -Wno-deprecated-declarations -Wno-unused-parameter -Wno-unused-variable -Wno-ignored-attributes ${EXTRA_CMAKE_CXX_FLAGS}" CACHE STRING "cxx_flags")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Vgcc_ntox86_64 -g -D__USESRCVERSION -D_QNX_SOURCE -DQNX ${EXTRA_CMAKE_C_FLAGS}" CACHE STRING "c_flags")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Vgcc_ntox86_64 -g -D__USESRCVERSION -D_QNX_SOURCE -DQNX -DTIXML_USE_STL -DEIGEN_MPL2_ONLY -Wno-deprecated-declarations -Wno-unused-parameter -Wno-unused-variable -Wno-ignored-attributes -fsized-deallocation ${EXTRA_CMAKE_CXX_FLAGS}" CACHE STRING "cxx_flags")

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -vv")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Y_cxx -std=c++14")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--build-id=md5 -lang-c++ -lsocket ${EXTRA_CMAKE_LINKER_FLAGS}" CACHE STRING "exe_linker_flags")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--build-id=md5 -lang-c++ -lsocket ${EXTRA_CMAKE_LINKER_FLAGS}" CACHE STRING "so_linker_flags")

if(NOT DEFINED CMAKE_PREFIX_PATH)
  if(NOT "$ENV{CMAKE_PREFIX_PATH}" STREQUAL "")
    string(REPLACE ":" ";" CMAKE_PREFIX_PATH $ENV{CMAKE_PREFIX_PATH})
  endif()
endif()
#---------------------------TEST--------------
#set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/src/apex_tools/apex_cmake/cmake/)
#---------------------------------------------------
set(CMAKE_FIND_ROOT_PATH ${CMAKE_PREFIX_PATH};${QNX_TARGET};${QNX_TARGET}/x86_64)
#set(CMAKE_FIND_ROOT_PATH ${CMAKE_PREFIX_PATH})

#set(CMAKE_SKIP_RPATH TRUE CACHE BOOL "If set, runtime paths are not added when using shared libraries.")
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

#if (NOT DEFINED TARGET_INSTALL_DIR)
#    # TARGET_INSTALL_DIR must point to where your ROS workspace will
#    # get installed on the target. The default setting is to overlay
#    # the workspace on top of the core ROS installation in
#    # /opt/ros/kinetic.
#    set(TARGET_INSTALL_DIR "/opt/ros/kinetic")
#endif()
#message(STATUS "ROS workspace installation directory is: ${TARGET_INSTALL_DIR}")

# macro to find programs on the host OS
macro( find_host_program )
  set( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )
  set( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER )
  set( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER )
  set( CMAKE_FIND_ROOT_PATH_MODE_PACKAGE NEVER )
  find_program( ${ARGN} )
  set( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY )
  set( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
  set( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY )
  set( CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY )
endmacro()

# macro to find packages on the host OS
macro( find_host_package )
  set( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )
  set( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER )
  set( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER )
  set( CMAKE_FIND_ROOT_PATH_MODE_PACKAGE NEVER )
  find_package( ${ARGN} )
  set( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY )
  set( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
  set( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY )
  set( CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY )
endmacro()
