# LedScreenProjectSEVRI
*a project with SEVRI, to diplay smileys on a LED display.*  
**PLEASE, BE AWARE THAT THIS REPO IS IN PROGRESS. It doesn't work yet. Thanks**

The project was in majority made withe an huge help of Michael VO, for SEVRI. We use a Raspberry Pie, with AdaFruit to link the raspberry to a LED Display.

## The purpose
The raspberry is link to an automaton that gives in real time how is the production (bad, middle, good). In the LED display, a corresponding smiley is displayed. There are 8 smileys displayed, corresponding to the 8 past informations. There is also a displayed number corresponding to the speed (communicated by the automaton too).
We used 4 RGB serial P5-LED panels with the Raspberry Pi.

## about the implementation

We used an existing project to use the LED matrix (LED-marix library), downloaded from https://github.com/hzeller/rpi-rgb-led-matrix.git (last download: 03/07/2020 - SHA 6668ff8b55fcfe5ea36cf22aa48155c9ced177a9).
The LED-matrix library is (c) Henner Zeller h.zeller@acm.org, licensed with GNU General Public License Version 2.0 (which means, if you use it in a product somewhere, you need to make the source and all your modifications available to the receiver of such product so that they have the freedom to adapt and improve).

We created the file code_leds.cpp and the images corresponding to the smileys.


## To run the program

You can download the program from a terminal with the command line bellow (it will create the repository in your current path):
>git clone --recurse-submodules -j8  https://github.com/canneltigrou/LedScreenProjectSEVRI.git
  
>cd LedScreenProjectSEVRI

To compile the project: 
>make -C SEVRI_pgrm

To clean (it will run clean for the submodule, and for our project):
>make clean

lets create our script.sh in the root of the raspberry, with the right path.
Please verify that you don't have any important file named script.sh in your root (since it will be replaced if you have some)
Please verify your terminal is in your folder LedScreenProjectSEVRI, then run these command lines into your terminal
>echo '#!/bin/bash' > script.sh
  
>echo sudo .$(pwd)'/SEVRI_pgrm/code_leds --led-chain=2 -D 1 --led-rows=32 --led-cols=128 --led-brightness=50 --led-no-hardware-pulse --led-gpio-mapping=adafruit-hat --led-slowdown-gpio=4 --led-daemon '$(pwd)'/SEVRI_pgrm/smile.ppm' >> script.sh
  
>echo sudo .$(pwd)'/SEVRI_pgrm/test_input > /dev/null' >> script.sh
  
Lets give all the permissions to the script:
>chmod 777 script.sh 
  
>sudo cp script.sh /


You can run the script:
>bash /script.sh






