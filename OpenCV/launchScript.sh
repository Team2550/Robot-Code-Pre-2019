#!/bin/bash
/home/pi/mjpg-streamer/mjpg-streamer/mjpg_streamer -i "/home/pi/mjpg-streamer/mjpg-streamer/input_uvc.so -y -f 30" -o "/home/pi/mjpg-streamer/mjpg-streamer/output_http.so -w ./www" -b
source /usr/local/bin/virtualenvwrapper.sh
workon cv
python ~/targetTracking.py
deactivate