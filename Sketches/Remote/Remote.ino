// -------------------------------------------------------------------------------------------------
//  Sketch     Default sketch for remote
//  Platform   Freenove Drone Remote Shield (for Arduino/Genuino Uno)
//  Brief      This sketch is used to send control signals to a receiver for drone.
//             Open Serial Monitor (baud rate 115200) to get detailed information and data.
//  Author     Ethan Pan @ Freenove (support@freenove.com)
//  Date       2018/06/08
//  Copyright  Copyright © Freenove (http://www.freenove.com)
//  License    Creative Commons Attribution ShareAlike 3.0
//             (http://creativecommons.org/licenses/by-sa/3.0/legalcode)
// -------------------------------------------------------------------------------------------------

#include <SPI.h>
#include "RF24.h"
#include <FlexiTimer2.h>
#include <EEPROM.h>

const int throttleHand = 0; // Position commande des gaz, si 0 commande à gauche

RF24 rf24 = RF24(9, 10); // Module de communication
const byte rf24Address[6] = { 'N', 'o', '0', '0', '1' };
const int rf24OutDataSize = 9;
byte rf24OutData[rf24OutDataSize];
const int rf24WriteInterval = 20; // Interval de communication 20ms

const int tonePin = 7; // Pin du beeper
const int toneFrequency = 2700; // Fréquence du beep
const int toneDuration = 25;

const int signalLedPin = 8;

const int voltagePin = A4;
const float voltageMultiplier = 1.0;

const int keysRowPins[] = {4, 5, 6, -1};
const int keysColumnPins[] = {4, 5, 6};
const int keysRowSize = sizeof(keysRowPins) / sizeof(int); // nombre d'éléments du tableau keysRowPins
const int keysColumnSize = sizeof(keysColumnPins) / sizeof(int); // nombre d'éléments du tableau keysColumnPins

const int switchsPins[] = {3, 2, A5}; // Pin des boutons
const int switchsSize = sizeof(switchsPins) / sizeof(int);
int lastSwitchsState[switchsSize];

int auxsState[switchsSize + 1];
const int auxsSize = sizeof(auxsState) / sizeof(int);

int joystickLeftYOffset;
int joystickLeftXOffset;
int joystickRightYOffset;
int joystickRightXOffset;

const int joystickOffsetStep = 10;

void setup() {
  Serial.begin(115200);
  Serial.println("Sketch is running...");

  pinMode(signalLedPin, OUTPUT);

  for (int i = 0; i < switchsSize; i++) {
    pinMode(switchsPins[i], INPUT_PULLUP);
    auxsState[i] = lastSwitchsState[i] = digitalRead(switchsPins[i]);
  }
  auxsState[switchsSize] = 0;

  if (!rf24.begin()) {
    Serial.println("Error: NRF24L01 module was not found!"); // Si le module de communication n'est pas trouvé
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
  rf24.stopListening();

  joystickLeftYOffset = eepromRead(0); // Lecture du décalage du Joystick dans la mémoire morte
  joystickLeftXOffset = eepromRead(1);
  joystickRightYOffset = eepromRead(2);
  joystickRightXOffset = eepromRead(3);
  Serial.print("Joystick offsets: Left Y: ");
  Serial.print(joystickLeftYOffset);
  Serial.print(", Left X: ");
  Serial.print(joystickLeftXOffset);
  Serial.print(", Right Y: ");
  Serial.print(joystickRightYOffset);
  Serial.print(", Right X: ");
  Serial.println(joystickRightXOffset);

  Serial.print("Throttle hand: ");
  if (throttleHand == 0) {
    Serial.println("left");
  }
  else {
    Serial.println("right");
  }
  for (int i = 0; i < throttleHand + 1; i++) {
    tone(tonePin, toneFrequency, toneDuration);
    delay(200);
  }

  FlexiTimer2::set(1, updateService); // call the updateService() function 3000 times per second
  FlexiTimer2::start();
}

void loop() {
  if (millis() % rf24WriteInterval != 0) {
    return;
  }

//{
 //digitalRead(switchsPins[2]) == HIGH;
 //pirouetteLeftY = 1023
 //delay 1500
 //pirouetteLeftY = 0
//}

  int joystickLeftY = constrain(analogRead(A0) + pirouetteLeftY + joystickLeftYOffset * joystickOffsetStep, 0, 1023); // Lecture de la position des Joysticks avec ajout du décalage
  int joystickLeftX = constrain(analogRead(A1) + joystickLeftXOffset * joystickOffsetStep, 0, 1023);
  int joystickRightY = constrain(analogRead(A2) + joystickRightYOffset * joystickOffsetStep, 0, 1023);
  int joystickRightX = constrain(analogRead(A3) + joystickRightXOffset * joystickOffsetStep, 0, 1023);
  

  int valueRoll, valuePitch, valueThrottle, valueYaw; //Roll : Roulis (inclnaison droite-gauche); Pitch : Aileron (inclinaison avant-arrière), Yaw : Dirrection droite-gauche, Throttle : Gaz
  
  if (throttleHand == 0) { // Si es gaz sont à gauche
    valueRoll = map(joystickRightY, 0, 1023, 2011, 988); // Translation du range 0-1024 en 2011-988 
    valuePitch = map(joystickRightX, 0, 1023, 2011, 988);
    valueThrottle = map(joystickLeftX, 0, 1023, 2011, 988);
    valueYaw = map(joystickLeftY, 0, 1023, 2011, 988);
  }
  else { // Si les gaz sont à droite
    valueRoll = map(joystickLeftY, 0, 1023, 988, 2011); // Translation du range 0-1024 en 988-2011 
    valuePitch = map(joystickRightX, 0, 1023, 988, 2011);
    valueThrottle = map(joystickLeftX, 0, 1023, 988, 2011);
    valueYaw = map(joystickRightY, 0, 1023, 988, 2011);
  }

  rf24OutData[0] = valueRoll / 256;
  rf24OutData[1] = valueRoll % 256; // modulo 
  rf24OutData[2] = valuePitch / 256;
  rf24OutData[3] = valuePitch % 256;
  rf24OutData[4] = valueThrottle / 256;
  rf24OutData[5] = valueThrottle % 256;
  rf24OutData[6] = valueYaw / 256;
  rf24OutData[7] = valueYaw % 256;

  for (int i = 0; i < auxsSize; i++) {
    bitWrite(rf24OutData[8], i, auxsState[i]); //rappel: rf24OutData est un tableau de "byte" = 8-"bit"
  } // rf24OutData[8] = value Aux 1,2,3,4

  if (rf24.write(rf24OutData, rf24OutDataSize)) {
    digitalWrite(signalLedPin, HIGH); // Si la fonction ne marche pas, allume la led
  }
  else {
    digitalWrite(signalLedPin, LOW);
  }
}

void eepromWrite(int index, int data) {
  EEPROM.write(index * 2, data >= 0 ? 0 : 1);
  EEPROM.write(index * 2 + 1, abs(data));
}

int eepromRead(int index) {
  byte data1 = EEPROM.read(index * 2);
  byte data2 = EEPROM.read(index * 2 + 1);

  if (data1 != 0 && data1 != 1) {
    data2 = 0;
  }

  return data1 == 0 ? data2 : -data2;
}

void updateService() {
  handleKey();
  handleSwitch();
  handleVoltage();
}

void handleVoltage() {
  static int counter = 0;
  if (counter++ % 1000 != 0) {
    return;
  }

  float voltage = getVoltage();
  Serial.print("Battery voltage: "); Serial.print(voltage); Serial.println("V.");
  if (voltage < 6.5 && voltage > 5.5) {
    tone(tonePin, toneFrequency, toneDuration);
    Serial.println("Low battery!");
  }
}

float getVoltage() {
  return analogRead(voltagePin) * 5.0 / 1023 * (10 + 6.2) / 6.2 * voltageMultiplier;
}

void handleSwitch() {
  int swithsState[switchsSize];

  for (int i = 0; i < switchsSize; i++) {
    swithsState[i] = digitalRead(switchsPins[i]);
    if (swithsState[i] != lastSwitchsState[i]) {
      Serial.print("Switch changed: ");
      switch (i) {
        case 0: // switch A
          Serial.print("A: ");
          break;
        case 1: // switch B
          Serial.print("B: ");
          break;
        case 2: // switch C
          Serial.print("C: ");
          break;
      }
      auxsState[i] = lastSwitchsState[i] = swithsState[i];
      Serial.println(swithsState[i]);
      tone(tonePin, toneFrequency, toneDuration);
    }
  }
}

void handleKey() {
  static int lastPressedKey = -1;
  int pressedKey = getPressedKey();

  if (pressedKey != -1 && lastPressedKey == -1) {
    Serial.print("Key pressed: ");
    switch (pressedKey) {
      case 1: // key 1
        joystickLeftYOffset += 1; // joystickLeftYOffset = joystickLeftYOffset + 1
        eepromWrite(0, joystickLeftYOffset);
        Serial.print("1. Joystick left Y offset: ");
        Serial.print(joystickLeftYOffset);
        break;
      case 2: // key 2
        joystickLeftYOffset -= 1;
        eepromWrite(0, joystickLeftYOffset);
        Serial.print("2. Joystick left Y offset: ");
        Serial.print(joystickLeftYOffset);
        break;
      case 3: // key 3
        joystickLeftXOffset += 1;
        eepromWrite(1, joystickLeftXOffset);
        Serial.print("3. Joystick left X offset: ");
        Serial.print(joystickLeftXOffset);
        break;
      case 5: // key 4
        joystickLeftXOffset -= 1;
        eepromWrite(1, joystickLeftXOffset);
        Serial.print("4. Joystick left X offset: ");
        Serial.print(joystickLeftXOffset);
        break;
      case 6: // key 5
        joystickRightYOffset += 1;
        eepromWrite(2, joystickRightYOffset);
        Serial.print("5. Joystick right Y offset: ");
        Serial.print(joystickRightYOffset);
        break;
      case 7: // key 6
        joystickRightYOffset -= 1;
        eepromWrite(2, joystickRightYOffset);
        Serial.print("6. Joystick right Y offset: ");
        Serial.print(joystickRightYOffset);
        break;
      case 9: // key 7
        joystickRightXOffset += 1;
        eepromWrite(3, joystickRightXOffset);
        Serial.print("7. Joystick right X offset: ");
        Serial.print(joystickRightXOffset);
        break;
      case 10: // key 8
        joystickRightXOffset -= 1;
        eepromWrite(3, joystickRightXOffset);
        Serial.print("8. Joystick right X offset: ");
        Serial.print(joystickRightXOffset);
        break;
      case 11: // key D
        auxsState[switchsSize] = 1;
        Serial.print("D");
        break;
    }
    Serial.print(". ");
    tone(tonePin, toneFrequency, toneDuration);
  }

  if (pressedKey == -1 && lastPressedKey != -1) {
    Serial.println("Key released.");
    auxsState[switchsSize] = 0;
  }

  lastPressedKey = pressedKey;
}

int getPressedKey() {
  int rowPin = -1;
  int columnPin = -1;
  static int pressedDuration = 0;

  for (int i = 0; i < keysRowSize; i++) {
    setKeysRowPin(i);
    int j = getKeysColumnPin(i);
    if (j != -1) {
      rowPin = i;
      columnPin = j;
    }
  }

  if (rowPin != -1 && columnPin != -1) {
    pressedDuration ++;
  }
  else {
    pressedDuration = 0;
  }

  if (pressedDuration >= 10) {
    return rowPin * keysColumnSize + columnPin;
  }
  else {
    return -1;
  }
}

void setKeysRowPin(int rowPin) {
  for (int i = 0; i < keysRowSize; i++) {
    if (keysRowPins[i] == -1) {
      continue;
    }
    if (i == rowPin) {
      pinMode(keysRowPins[i], OUTPUT);
      digitalWrite(keysRowPins[i], LOW);
    }
    else {
      pinMode(keysRowPins[i], INPUT_PULLUP);
    }
  }
}

int getKeysColumnPin(int rowPin) {
  for (int i = 0; i < keysColumnSize; i++) {
    if (i == rowPin) {
      continue;
    }
    if (digitalRead(keysColumnPins[i]) == LOW) {
      return i;
    }
  }
  return -1;
}

