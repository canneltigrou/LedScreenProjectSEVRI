#!/bin/bash

sudo ./home/pi/rpi-rgb-led-matrix-v2/examples-api-use/demo --led-chain=2 -D 1 --led-rows=32 --led-cols=128 --led-brightness=50 --led-no-hardware-pulse --led-gpio-mapping=adafruit-hat --led-slowdown-gpio=4 --led-daemon /home/pi/rpi-rgb-led-matrix-v2/examples-api-use/smile2.ppm
sudo ./home/pi/rpi-rgb-led-matrix-v2/examples-api-use/test_input > /dev/null