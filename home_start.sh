#!/bin/sh
cd driver
insmod am2311.ko
insmod hs38b_ir_driver.ko
insmod s5pv210-gpio.ko
cd ..

cd mjpeg/mjpg
./start.sh &
cd ../..
echo "start === mjpg"

cd mjpeg/mjpg-streamer
./start_uvc_yuv.sh &
cd ../..

cd web/boa/
./boa
cd ../..
echo "start boa"


cd execute
./home_gtk
