#!/bin/bash
#!/snap/bin/expect


echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
echo "Defining variables and building rtos files"
echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

APP=build_qnx
WHOAMI=root
BASEDIR=$(pwd)
SSH_PSK="root"

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

echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
echo "Building CMake"
echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"

cmake -B ${APP} -S . -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=qnx.nto.toolchain2.cmake

cmake --build ${APP}


echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
echo "Building GUI with qmake"
echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"


QMAKE=/home/dev/opt/qt_for_qnx/bin/qmake

if [ -z $(which $QMAKE) ]
then
   if [ -z $(which /home/dev/opt/qnx/qt_for_qnx/bin/qmake) ]
   then
      echo "No qmake defined"
      exit 1
   else
      QMAKE=/home/dev/opt/qnx/qt_for_qnx/bin/qmake
   fi
   
fi



mkdir -p ${APP}/gui/qnx/release && cd ${APP}/gui/qnx/release

OUTPUT_BUILD=$(pwd)


$QMAKE /home/dev/dev/rtos_vehicule_monitoring/gui/gui.pro -spec qnx-x86-64-qcc CONFIG+=qtquickcompiler CONFIG-=separate_debug_info

$QNX_HOST/usr/bin/make -f $OUTPUT_BUILD/Makefile qmake_all


$QNX_HOST/usr/bin/make -j8


cd $BASEDIR

echo Sending data to remote address ${QNX_ADDRESS}

sshpass -p ${SSH_PSK} scp -r ${APP} ${WHOAMI}@${QNX_ADDRESS}:/opt/apps/ 

# Optional: direct output stdout and stderr to null
# >/dev/null 2>&1



sshpass -p ${SSH_PSK} ssh ${WHOAMI}@${QNX_ADDRESS} "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/apps/${APP}/lib/rtos_common && cd /opt/apps/${APP}"

sshpass -p ${SSH_PSK} ssh ${WHOAMI}@${QNX_ADDRESS}  cd /opt/apps/$APP