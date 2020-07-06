#!/bin/bash

sudo ./home/pi/LedScreenProjectSEVRI/SEVRI_pgrm/code_leds --led-chain=2 -D 1 --led-rows=32 --led-cols=128 --led-brightness=50 --led-no-hardware-pulse --led-gpio-mapping=adafruit-hat --led-slowdown-gpio=4 --led-daemon /home/pi/LedScreenProjectSEVRI/SEVRI_pgrm/smile.ppm
sudo ./home/pi/LedScreenProjectSEVRI/SEVRI_pgrm/test_input > /dev/null