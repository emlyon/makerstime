// -------------------------------------------------------------------------------------------------
//  File       Output PPM signal
//  Brief      This file is used to output PPM signal.
//  Author     Ethan Pan @ Freenove (support@freenove.com)
//  Date       2018/06/08
//  Copyright  Copyright © Freenove (http://www.freenove.com)
//  License    Creative Commons Attribution ShareAlike 3.0
//             (http://creativecommons.org/licenses/by-sa/3.0/legalcode)
// -------------------------------------------------------------------------------------------------

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <Arduino.h>

#define PPM_CHANNELS 8                      // The number of chanels
static uint_fast16_t _frameLength = 22500;  // The PPM frame width (all channels) in microseconds (1ms = 1000us), defaults to 22500
static uint_fast16_t _pulseLength = 300;    // The pulse length of any individual channel in microseconds, defualts to 300
static uint_fast8_t  _polarity    = 1;      // The polarity of pulses, defaults to HIGH (HIGH or LOW / 0 or 1)

// Initializes the PPM output by setting up timer1 and initializing the output pin
void ppmOut_init(int outPin);
// Changes the PPM parameters to non-default settings
void ppmOut_config(uint_fast16_t frameLength, uint_fast16_t pulseLength, uint_fast8_t polarity);
// Begins the PPM output (assumes 16Mhz processor)
void ppmOut_begin();
// Stops the PPM output
void ppmOut_stop();
// Write a value to a specific channel (No bounds checking on channel or value!)
void ppmOut_write(uint_fast8_t channel, uint_fast16_t value);
// Write a value to ALL channels (No bounds checking on value!)
void ppmOut_writeAll(uint_fast16_t value);

#ifdef __cplusplus
}
#endif

