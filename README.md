# lightbox-control
A project for my Arduino-based RGB LED ambient lighting

## Overview
This thing was supposed to run on Arduino machine, listen to a serial port for the colors stream data and use this data to set colors on a connected RGB LED strip

## Use case
Client application would capture screen (~30 times a second), then analyze this screenshot, extract 24 averaged colors along the edge of the screenshot and send them to the Arduino over the serial port

## Outcome
Well, my RGB LED strip (or TLC controller) didn't hold the colors stable enough: they would flicker once in a while, or sometimes even hang completely. So this project didn't go well because of the hardware issues
