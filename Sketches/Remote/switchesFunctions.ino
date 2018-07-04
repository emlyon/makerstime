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
