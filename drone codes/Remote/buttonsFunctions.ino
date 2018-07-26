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
        pressedDuration++;
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
