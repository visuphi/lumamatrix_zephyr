.. _adafruit_mcp9808:

ZHAW Luma Matrix Shield
#######################

Overview
********

The `ZHAW Luma Matrix Shield`_ is a 8x8 grid of WS2812B RGB LEDs, 
which Adafruit refers to as “neopixels”. 
There is a joystick and a switch on the backside.
The shield id designed to be compatible with Raspberry Pi Pico

.. figure:: zhaw_lumamatrix.png
   :align: center
   :alt: ZHAW Luma Matrix Shield

   ZHAW Luma Matrix Shield (Credit: ZHAW)


Requirements
************

This shield can only be used with a Raspbery Pi Pico

Programming
***********

Set ``--shield zhaw_lumamatrix`` when you invoke ``west build``.  For example
when running the :zephyr:code-sample:`led_strip` LED-Strip sample:

.. zephyr-app-commands::
   :zephyr-app: samples/drivers/led/led_strip
   :board: rpi_pico
   :shield: zhaw_lumamatrix
   :goals: build

