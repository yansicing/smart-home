#!/bin/sh

CGI_DIR=../www/cgi-bin/

#cd common
#make clean
#make 
#cd ..

cd get_led_status
make clean
make
#cp get_led_status.cgi $CGI_DIR
cd ..


cd switch_led
make clean
make
#cp switch_led.cgi $CGI_DIR
cd ..

cd send_message
make clean
make
#cp send_message.cgi $CGI_DIR
cd ..


cd login_check
make clean
make
#cp login_check.cgi $CGI_DIR
cd ..


cd get_humiture
make clean
make
#cp get_humiture.cgi $CGI_DIR
cd ..


cd humiture_setting
make clean
make
#cp humiture_setting.cgi $CGI_DIR
cd ..




