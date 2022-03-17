#!/bin/bash
#!/snap/bin/expect

APP=build_qnx
WHOAMI=root
QNX_ADDRESS=192.168.187.128


if [ -d "${APP}" ]
then
   rm -rf ${APP}
fi 

cmake -B ${APP} -S . -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=qnx.nto.toolchain2.cmake

cmake --build ${APP}

echo Sending data to remote address ${QNX_ADDRESS}

sshpass -p "qnxuser" scp -r ${APP} ${WHOAMI}@${QNX_ADDRESS}:~/ 

sshpass -p "qnxuser" ssh ${WHOAMI}@${QNX_ADDRESS} "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/${WHOAMI}/${APP}/lib/rtos_common && cd ${APP} &&  ./rtos_vehicule_monitoring"
