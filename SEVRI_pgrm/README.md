# LedScreenProjectSEVRI
*a project with SEVRI, to diplay smileys on a LED display.*
**PLEASE, BE AWARE THAT THIS REPO IS IN PROGRESS. It doesn't work yet. Thanks**

## To display the smileys on the panels
We have the files code_leds.cpp that uses the library ../rpi-rgb-led-matrix to work
It reads the entries on a file, and display the corresponding smileys on the pannel using the adafruit system.
Please, refers to the ../rpi-rgb-led-matrix/README.md to see the different commands to execute to configure the raspberry.

## To read the entries from the automaton
We have the files ABE_IoPI.cc, ABE_IoPI.h that comes from  https://github.com/abelectronicsuk/ABElectronics_CPP_Libraries/tree/master/IOPi
We used a C++ Library to use with IO Pi Raspberry Pi expansion board from https://www.abelectronics.co.uk
### IO Pi
This library reads and writes to the [IO Pi Plus](https://www.abelectronics.co.uk/p/54/io-pi-plus "IO Pi Plus"), [IO Pi Zero](https://www.abelectronics.co.uk/p/71/io-pi-zero "IO Pi Zero") and [IO Pi](https://www.abelectronics.co.uk/kb/article/1042/io-pi "IO Pi").

Required package:
>apt-get install libi2c-dev



## To build the program

To compile the project: 
>make -C SEVRI_pgrm

To clean (it will run clean for the submodule, and for our project):
>make clean





