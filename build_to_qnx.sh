#!/bin/bash
#!/snap/bin/expect


echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
echo "Defining variables and building rtos files"
echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

APP=build_qnx
WHOAMI=root

if [ -z "$1" ]
then
   QNX_ADDRESS=172.16.50.129
else
   QNX_ADDRESS="$1"
fi

if [ -z "${QNX_HOST}" ]
then
 echo "Undefined QNX_HOST env variable"
 echo "Finding environment variable export file..."
 echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
   if [ -z $(find ~/opt/qnx -type f -name qnx*.sh | wc -l) ]
   then
      echo "File not found and must be sourced manually"
      exit 1
#   elif [ $(find ~/opt/qnx/qnx700 -type f -name qnx*.sh | wc -l) > 1 ]
#   then
      
   else
      echo "Sourcing files..."
      . $(find ~/opt/qnx/qnx700 -type f -name qnx*.sh)
      echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
      sleep 1
   fi
fi

if [ "$(ping $QNX_ADDRESS -c 1 | awk 'NR==5 {print $4}')" != "1" ] 
then
 echo Unreacheable remote address $QNX_ADDRESS
 exit 1
fi



if [ -d "${APP}" ]
then
   rm -rf ${APP}
fi 

cmake -B ${APP} -S . -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=qnx.nto.toolchain2.cmake

cmake --build ${APP}

echo Sending data to remote address ${QNX_ADDRESS}

sshpass -p "root" scp -r ${APP} ${WHOAMI}@${QNX_ADDRESS}:~/ >/dev/null 2>&1

# sshpass -p "qnxuser" ssh ${WHOAMI}@${QNX_ADDRESS} "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/${WHOAMI}/${APP}/lib/rtos_common && cd ${APP} &&  ./rtos_vehicule_monitoring"
