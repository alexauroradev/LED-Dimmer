# LED-Dimmer

## Protocol

HC-06 module (Bluetooth) is reading information sent to it in lines. The line should be of the format:

`CC [param]`

* `CC` is two chars that represent a command
* param is an optional parameter that is different for different commands

The list of supported commands include:
* `B1`, `B2` -- change the brightness of the LED strip. Must be followed by an integer from 0 to 255 (brightness). Uses default switching time, recorded on Arduino.
* `ST` -- set the switching time. Must be followed by an integer (switching time in ms)
