#!/bin/bash 
source /etc/environment

echo EBB-PRU-RTC4 > /sys/devices/bone_capemgr.9/slots
echo BB-UART4 > /sys/devices/bone_capemgr.9/slots
stty -echo
. ~/.bash_profile


