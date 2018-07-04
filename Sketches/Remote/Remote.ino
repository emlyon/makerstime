// -------------------------------------------------------------------------------------------------
//  Sketch     Default sketch for remote
//  Platform   Freenove Drone Remote Shield (for Arduino/Genuino Uno)
//  Brief      This sketch is used to send control signals to a receiver for drone.
//       Open Serial Monitor (baud rate 115200) to get detailed information and data.
//  Author     Ethan Pan @ Freenove (support@freenove.com)
//  Date       2018/06/08
//  Copyright  Copyright © Freenove (http://www.freenove.com)
//  License    Creative Commons Attribution ShareAlike 3.0
//       (http://creativecommons.org/licenses/by-sa/3.0/legalcode)
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

const int switchsPins[] = {3, 2, A5}; // Pin des switchs
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

    if( auxsState[ 2 ] == HIGH ){
        // sendData( avec des valeurs fixées, 1200 );
    }
    else if( auxState[ 3 ] == HIGH ){
        // sendData( avec des valeurs fixées, 1200 );
    }
    else{
        int joystickLeftY = constrain(analogRead(A0) + joystickLeftYOffset * joystickOffsetStep, 0, 1023); // Lecture de la position des Joysticks avec ajout du décalage
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
        sendData( valueRoll, valuePitch, valueThrottle, valueYaw, auxsState[ 0 ], auxsState[ 1 ] );
    }
}

void sendData(int valueRoll, int valuePitch, int valueThrottle, int valueYaw, int aux1, int aux2, int duration = 0 ){
    // rf24OutData est un array de bytes (octets, qui peut donc stocker une valeur de 0 à 255 )
    // donc pour stocker des valeurs de 988 à 2011
    rf24OutData[0] = valueRoll / 256; // on divise la valeur par 256 et on stocke la valeur entière
    rf24OutData[1] = valueRoll % 256; // puis on stocke le reste de la division par 256
    rf24OutData[2] = valuePitch / 256;
    rf24OutData[3] = valuePitch % 256;
    rf24OutData[4] = valueThrottle / 256;
    rf24OutData[5] = valueThrottle % 256;
    rf24OutData[6] = valueYaw / 256;
    rf24OutData[7] = valueYaw % 256;
    // rf24OutData[0 à 7] = valeurs de 988 à 2011 en alternant valeur entière de la division par 256 et reste de la division par 256

    //rappel: rf24OutData est un tableau de "byte" = 8-"bit"
    bitWrite(rf24OutData[8], i, aux1);
    bitWrite(rf24OutData[8], i, aux2);
    bitWrite(rf24OutData[8], i, 0);
    bitWrite(rf24OutData[8], i, 0);
    // rf24OutData[8] = value Aux 1,2

    // send data to rf module

    long timestamp = millis();
    do{
        if (rf24.write(rf24OutData, rf24OutDataSize)) {
            digitalWrite(signalLedPin, HIGH); // Si la fonction marche, allume la led
        }
        else {
            digitalWrite(signalLedPin, LOW);
        }
    } while( millis() < timestamp + duration );
}

// updateService is called 3000 times by second via FlexiTimer library
void updateService() {
    handleKey();
    handleSwitch();
    handleVoltage();
}
