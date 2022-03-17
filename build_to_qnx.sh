#!/bin/bash
#!/snap/bin/expect

APP=build_qnx
WHOAMI=root

if [ -d "${APP}" ]
then
   rm -rf ${APP}
fi 

cmake -B ${APP} -S . -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=qnx.nto.toolchain2.cmake

cmake --build ${APP}

sshpass -p "qnxuser" scp -r ${APP} ${WHOAMI}@172.16.50.129:~/ 

sshpass -p "qnxuser" ssh ${WHOAMI}@172.16.50.129 "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/${WHOAMI}/${APP}/lib/rtos_common && cd ${APP} &&  ./rtos_vehicule_monitoring"