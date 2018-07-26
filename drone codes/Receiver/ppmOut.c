// -------------------------------------------------------------------------------------------------
//  File       Output PPM signal
//  Brief      This file is used to output PPM signal.
//  Author     Ethan Pan @ Freenove (support@freenove.com)
//  Date       2018/06/08
//  Copyright  Copyright © Freenove (http://www.freenove.com)
//  License    Creative Commons Attribution ShareAlike 3.0
//             (http://creativecommons.org/licenses/by-sa/3.0/legalcode)
// -------------------------------------------------------------------------------------------------

#include "ppmOut.h"

static int _outPin;                         // The output pin for the PPM output, must be a digital pin
static uint_fast16_t _ppmAR[PPM_CHANNELS];  // The array holds the values for the ppm signal

void ppmOut_init(int outPin) {
  // Center all the channels to the default value
  ppmOut_writeAll(1500);

  // Setup the output pin
  _outPin = outPin;
  pinMode(_outPin, OUTPUT);
  digitalWrite(_outPin, !_polarity);

  // Setup timer 1
  cli();
  // Clear out timer 1 config registers
  TCCR1A  = 0;
  TCCR1B  = 0;
  // Setup timer 1 for PPM output and enable interrupts
  OCR1A   = 100;           // Compare match register, change this
  TCCR1B |= (1 << WGM12);  // Turn on CTC mode (Clear Timer on Compare Match)
  TIMSK1 |= (1 << OCIE1A); // Enable timer compare interrupt
  sei();
}

void ppmOut_config(uint_fast16_t frameLength, uint_fast16_t pulseLength, uint_fast8_t polarity) {
  // Set the PPM parameter values
  _frameLength = frameLength;
  _pulseLength = pulseLength;
  _polarity    = polarity;

  // Set output pin to off state in case polarity changed
  digitalWrite(_outPin, !_polarity);
}

void ppmOut_begin() {
  cli();
  TCNT1 = 0;
  TCCR1B |= (1 << CS11);   // 8 prescaler: 0.5 microseconds at 16mhz
  sei();
}

void ppmOut_stop() {
  cli();
  TCCR1B &= (0 << CS11);  // Disable the timer, thereby disabline the PPM output
  sei();
}

void ppmOut_write(uint_fast8_t channel, uint_fast16_t value) {
  _ppmAR[channel] = value;
}

void ppmOut_writeAll(uint_fast16_t value) {
  // Set all values in the ppm array to the center value
  for (int i = 0; i < PPM_CHANNELS; i++) {
    _ppmAR[i] = value;
  }
}

ISR(TIMER1_COMPA_vect) {
  static boolean state = true;

  TCNT1 = 0;

  // Start pulse
  if (state) {
    digitalWrite(_outPin, _polarity);
    OCR1A = _pulseLength * 2;
    state = false;
  }
  // End pulse and calculate when to start the next pulse
  else {
    static byte currentChannel;
    static unsigned int calcRest;

    digitalWrite(_outPin, !_polarity);
    state = true;

    if (currentChannel >= PPM_CHANNELS) {
      currentChannel = 0;
      calcRest = calcRest + _pulseLength;
      OCR1A = (_frameLength - calcRest) * 2;
      calcRest = 0;
    }
    else {
      OCR1A = (_ppmAR[currentChannel] - _pulseLength) * 2;
      calcRest = calcRest + _ppmAR[currentChannel];
      currentChannel++;
    }
  }
}

