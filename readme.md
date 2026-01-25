# Zephyr for ZHAW Luma Matrix

This repository contains the shield files and the led-strip sample from zephyr.

## build

```
west build -b rpi_pico --shield zhaw_lumamatrix
```

## flash 

- hold boot button and connect the rpi_pico to the computer. 
- release boot button
- copy `zephyr.uf2` from the build folder onto the rpi_pico


## to-do's

- [x] add led-strip
- [x] add joystick 
- [x] add switch