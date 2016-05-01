# Cat Palace Lighting
[![Build badge](https://travis-ci.org/Leenix/Cat-Palace-Lighting.svg)](https://travis-ci.org/Leenix/Cat-Palace-Lighting)
[![Arduino badge](https://img.shields.io/badge/platform-Arduino-brightgreen.svg)](http://www.arduino.cc/)
[![License](http://www.gnu.org/graphics/gplv3-127x51.png)](http://www.gnu.org/licenses/gpl-3.0.en.html)

We have indoor cats, but we still want them to have the whole outdoor-cat lifestyle (I'm sure there's a dumb reality-TV show with the same plot). Anyway, the cats have an outdoor playpen that needs automatic lighting for when it gets dark. So here it is.

Once the ambient light is dark enough, the LED strip will turn on for 6 hours (typically between 1800 - 2359). The lamps will then turn off until the next day to start the cycle anew.

# Features

|Feature|Description|
|----|----|
|Ambient light sensor| To check how much sun/not sun these cats are getting in their outdoor resort|
|RGB light controller| A lighting controller needs a light (in exciting 24-bit colour)|
|LCD Screen| So you can see what the controller is doing with your human eyes
|Power monitor| Keep tabs on your power consumption (so we can send our feline friends the bill)
|`Coming soon` Real-time clock| For keeping it real (in a time kind of way)

# Docs

- [Parts list](/doc/parts.md) - Get all the things you need to put the controller together
- [Installation] - Put the controller together
- [Program the Arduino](/doc/Program-the-Arduino.md) - Put the code onto the board

# [License](http://www.gnu.org/licenses/gpl-3.0.en.html)

This project is licensed under the terms of the GNU General Public License v3.0.
