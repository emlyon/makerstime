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
