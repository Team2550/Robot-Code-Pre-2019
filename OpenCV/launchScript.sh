#!/bin/bash
source /usr/local/bin/virtualenvwrapper.sh
workon cv
python ~/targetTracking.py
deactivate