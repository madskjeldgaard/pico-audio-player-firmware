#pragma once

#include <Arduino.h>

namespace picosampler {

// Note these are for the Pico, and the pinouts have physical pin numbers and
// GPIO numbers. These are the gpio numbers used.

// I2S pins
constexpr auto i2sDIN = 28;
constexpr auto i2sLCK = 27;
constexpr auto i2sBCK = 26;
// Connect the DAC SCK pin to GND.

// The CS pin for the SD card
// reader
constexpr int sdCSPin = 17;
constexpr int sdSCKPin = 18;
constexpr int sdMISOPin = 16;
constexpr int sdMOSIPin = 19;

} // namespace picosampler
