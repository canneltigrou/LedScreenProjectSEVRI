# LedScreenProjectSEVRI
*a project with SEVRI, to diplay smileys on a LED display.*  

The project was in majority made with a huge help of Michael VO, for SEVRI. We use a Raspberry Pie, with AdaFruit to link the raspberry to a LED Display.

## The purpose
The raspberry is linked to an automaton that gives in real time how is the production (bad, middle, good). In the LED display, a corresponding smiley is displayed. There are 8 smileys displayed, corresponding to the 8 past informations. There is also a displayed number corresponding to the speed (communicated by the automaton too).
We used 4 RGB serial P5-LED panels with the Raspberry Pi.  
<img src="https://github.com/canneltigrou/LedScreenProjectSEVRI/blob/master/SEVRI_pgrm/example/IMG_20200822_113504.jpg" width="500">

## About the implementation

We used an existing project to use the LED matrix (LED-marix library), downloaded from https://github.com/hzeller/rpi-rgb-led-matrix.git (last download: 03/07/2020 - SHA 6668ff8b55fcfe5ea36cf22aa48155c9ced177a9).
The LED-matrix library is (c) Henner Zeller h.zeller@acm.org, licensed with GNU General Public License Version 2.0 (which means, if you use it in a product somewhere, you need to make the source and all your modifications available to the receiver of such product so that they have the freedom to adapt and improve).

We created the file code_leds.cpp and the images corresponding to the smileys.

We also used an [IO Pi](https://www.abelectronics.co.uk/kb/article/1042/io-pi "IO Pi") library to reads and writes to the ABElectrnics device that we use to communicate with the automaton.


## To build the program

You can download the program from a terminal with the command line bellow (it will create the repository in your current path):
>git clone --recurse-submodules -j8  https://github.com/canneltigrou/LedScreenProjectSEVRI.git
  
>cd LedScreenProjectSEVRI

To compile the project: 
>make -C SEVRI_pgrm

To clean (it will run clean for the submodule, and for our project):
>make clean


## Launch the program

For our project, it is important to launch the program at the raspberry starting. To do that, we use a script (script.sh) that launches the 2 programs in parallel, and we add a crontab to execute the script when the rasberry is starting. You can see an example of crontab on SEVRI_pgrm/example

### creation of script.sh
we create our script.sh in the home of the raspberry, with the right path.
Please verify that you don't have any important file named script.sh in your root (since it will be replaced if you have some)
Please verify your terminal is in your folder LedScreenProjectSEVRI, then run these command lines into your terminal
>echo '#!/bin/bash' > script.sh
  
>echo cd $(pwd)'/SEVRI_pgrm' >> script.sh



>echo ./code_leds -D 1 --led-daemon >> script.sh
  
>echo './test_input > /dev/null' >> script.sh

Lets give all the permissions to the script:
>chmod 777 script.sh 
  
>sudo cp script.sh ~


You can run the script from your home:
>cd ~
>./script.sh


## kill the program
You can kill the program test_input (which read the inputs from the automaton) just by ctrl-C  
However the second program that displays the smileys is launch in parallel. You can find its pid by the command  
>ps aux | grep 'code_led'  

then you can kill it with its pid  
>sudo kill *its_pid*





