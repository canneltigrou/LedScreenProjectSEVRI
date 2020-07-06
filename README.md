# LedScreenProjectSEVRI
*a project with SEVRI, to diplay smileys on a LED display.*

The project was in majority made withe an huhe help of Michael VO, for SEVRI. We use a Raspberry Pie, with AdaFruit to link the raspberry to a LED Display.

## The purpose
The raspberry is link to an automaton that gives in real time how is the production (bad, middle, good). In the LED display, a corresponding smiley is displayed. There are 8 smileys displayed, corresponding to the 8 past informations. There is also a displayed number corresponding to the speed (communicated by the automaton too).
We used 4 RGB serial P5-LED panels with the Raspberry Pi.

## about the implementation

We used an existing project to use the LED matrix (LED-marix library), downloaded from https://github.com/hzeller/rpi-rgb-led-matrix.git (last download: 03/07/2020 - SHA 6668ff8b55fcfe5ea36cf22aa48155c9ced177a9).
The LED-matrix library is (c) Henner Zeller h.zeller@acm.org, licensed with GNU General Public License Version 2.0 (which means, if you use it in a product somewhere, you need to make the source and all your modifications available to the receiver of such product so that they have the freedom to adapt and improve).

We created the file code_leds.cpp and the images corresponding to the smileys.


## To run the program

To compile the project: 
>make -C SEVRI_pgrm



