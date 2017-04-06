#!/bin/bash
mjpg_streamer -i "input_uvc.so -y -f 20 -r 640x360" -o "output_http.so -w /home/pi/mjpg-streamer/mjpg-streamer/www" -b
v4l2-ctl -c exposure_auto=1
v4l2-ctl -c exposure_absolute=0
source /usr/local/bin/virtualenvwrapper.sh
workon cv

while true
do
	python ~/Nathan\ -\ tracking.py
done

deactivate
killall mjpg_streamer
#sudo shutdown -h now
