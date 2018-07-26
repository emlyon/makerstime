void handleSwitch() {
    int switchsState[switchsSize];

    for (int i = 0; i < switchsSize; i++) {
        switchsState[i] = digitalRead(switchsPins[i]);
        if (switchsState[i] != lastSwitchsState[i]) {
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
            auxsState[i] = lastSwitchsState[i] = switchsState[i];
            Serial.println(switchsState[i]);
            tone(tonePin, toneFrequency, toneDuration);
        }
    }
}
