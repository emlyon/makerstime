// -------------------------------------------------------------------------------------------------
//  Sketch     Default sketch for receiver
//  Platform   Freenove Drone Receiver (Arduino Pro or Pro Mini)
//  Brief      This sketch is used to receive control signals from a remote for drone.
//             Open Serial Monitor (baud rate 115200) to get detailed information and data.
//  Author     Ethan Pan @ Freenove (support@freenove.com)
//  Date       2018/06/08
//  Copyright  Copyright © Freenove (http://www.freenove.com)
//  License    Creative Commons Attribution ShareAlike 3.0
//             (http://creativecommons.org/licenses/by-sa/3.0/legalcode)
// -------------------------------------------------------------------------------------------------

#include <SPI.h>
#include "RF24.h"
#include "ppmOut.h"

RF24 rf24 = RF24(9, 10);
const byte rf24Address[6] = { 'N', 'o', '0', '0', '1' };
const int rf24InDataSize = 9;
byte rf24InData[rf24InDataSize];

const int signalLedPin = 2;
const int signalPin = 3;

enum SignalChannel { ROLL, PITCH, THROTTLE, YAW, AUX1, AUX2, AUX3, AUX4 };

unsigned long currentSignalReceived = 1;

void setup() {
  Serial.begin(115200);
  Serial.println("Sketch is running...");

  pinMode(signalLedPin, OUTPUT);

  if (!rf24.begin()) {
    Serial.println("Error: NRF24L01 module was not found!");
    while (1) {
      digitalWrite(signalLedPin, HIGH);
      delay(500);
      digitalWrite(signalLedPin, LOW);
      delay(500);
    }
  }
  rf24.setPALevel(RF24_PA_MAX);
  rf24.setDataRate(RF24_1MBPS);
  rf24.setRetries(0, 15);
  rf24.openWritingPipe(rf24Address);
  rf24.openReadingPipe(1, rf24Address);
  rf24.startListening();

  ppmOut_init(signalPin);
  ppmOut_write(SignalChannel::ROLL, 1500);
  ppmOut_write(SignalChannel::PITCH, 1500);
  ppmOut_write(SignalChannel::THROTTLE, 1000);
  ppmOut_write(SignalChannel::YAW, 1500);
  ppmOut_write(SignalChannel::AUX1, 1500);
  ppmOut_write(SignalChannel::AUX2, 1500);
  ppmOut_write(SignalChannel::AUX3, 1500);
  ppmOut_write(SignalChannel::AUX4, 1500);
  ppmOut_begin();
}

void loop() {
  if (rf24.available()) {
    rf24.read(rf24InData, rf24InDataSize);

    int valueRoll = rf24InData[0] * 256 + rf24InData[1];
    int valuePitch = rf24InData[2] * 256 + rf24InData[3];
    int valueThrottle = rf24InData[4] * 256 + rf24InData[5];
    int valueYaw = rf24InData[6] * 256 + rf24InData[7];
    int valueAux1 = bitRead(rf24InData[8], 0) ? 2011 : 988;
    int valueAux2 = bitRead(rf24InData[8], 1) ? 2011 : 988;
    int valueAux3 = bitRead(rf24InData[8], 2) ? 2011 : 988;
    int valueAux4 = bitRead(rf24InData[8], 3) ? 2011 : 988;

    ppmOut_write(SignalChannel::ROLL, valueRoll);
    ppmOut_write(SignalChannel::PITCH, valuePitch);
    ppmOut_write(SignalChannel::THROTTLE, valueThrottle);
    ppmOut_write(SignalChannel::YAW, valueYaw);
    ppmOut_write(SignalChannel::AUX1, valueAux1);
    ppmOut_write(SignalChannel::AUX2, valueAux2);
    ppmOut_write(SignalChannel::AUX3, valueAux3);
    ppmOut_write(SignalChannel::AUX4, valueAux4);

    Serial.print("RF24 received: ");
    Serial.print("Roll: "); Serial.print(valueRoll);
    Serial.print(", Pitch: "); Serial.print(valuePitch);
    Serial.print(", Throttle: "); Serial.print(valueThrottle);
    Serial.print(", Yaw: "); Serial.print(valueYaw);
    Serial.print(", Aux1: "); Serial.print(valueAux1);
    Serial.print(", Aux2: "); Serial.print(valueAux2);
    Serial.print(", Aux3: "); Serial.print(valueAux3);
    Serial.print(", Aux4: "); Serial.println(valueAux4);

    currentSignalReceived = millis();
    digitalWrite(signalLedPin, HIGH);
  }

  if (millis() - currentSignalReceived > 20) {
    digitalWrite(signalLedPin, LOW);
  }

  if (currentSignalReceived != 0) {
    if (millis() - currentSignalReceived > 500) {
      ppmOut_write(SignalChannel::ROLL, 1500);
      ppmOut_write(SignalChannel::PITCH, 1500);
      ppmOut_write(SignalChannel::THROTTLE, 500);
      ppmOut_write(SignalChannel::YAW, 1500);
      currentSignalReceived = 0;
      Serial.println("No signal!");
    }
  }
}

