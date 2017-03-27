#!/bin/bash
/home/pi/mjpg-streamer/mjpg-streamer/mjpg_streamer -i "/home/pi/mjpg-streamer/mjpg-streamer/input_uvc.so -y -f 20 -r 640x360" -o "/home/pi/mjpg-streamer/mjpg-streamer/output_http.so -w /home/pi/mjpg-streamer/mjpg-streamer/www" -b
v4l2-ctl -c exposure_auto=1
v4l2-ctl -c exposure_absolute=0
source /usr/local/bin/virtualenvwrapper.sh
workon cv

while true
do
	python ~/targetTracking.py
done

deactivate
killall mjpg_streamer
#sudo shutdown -h now
