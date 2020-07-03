# LedScreenProjectSEVRI
a project with SEVRI, to diplay smileys on a LED display.

The project was in majority made by Michael VO, for SEVRI. We use a Raspberry Pie, with AdaFruit to link the raspberry to a LED Display.

## The purpose
The raspberry is link to an automaton that gives in real time how is the production (bad, middle, good). In the LED display, a corresponding smiley is displayed. There are 8 smileys displayed, corresponding to the 8 past informations. There is also a displayed number corresponding to the speed (communicated by the automaton too).

## about the implementation

We used an existing project to use the LED matrix, downloaded from https://github.com/hzeller/rpi-rgb-led-matrix.git (last download: 03/07/2020 - SHA 6668ff8b55fcfe5ea36cf22aa48155c9ced177a9).
We created the file code_leds.cpp and the images corresponding to the smileys.



