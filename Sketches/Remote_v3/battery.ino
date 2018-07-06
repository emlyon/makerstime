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
