#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/apps/build_qnx/lib/rtos_common

/etc/graphics-startup.sh &

/opt/apps/build_qnx/rtos_vehicule_monitoring

