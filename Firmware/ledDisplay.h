#ifndef LEDDISPLAY_H
#define LEDDISPLAY_H

// DP G F E D C B A
const unsigned char alpha[] = {
    0b01110111, // A
    0b01111100, // b
    0b00111001, // C
    0b01011110, // d
    0b01111001, // E
    0b01110001, // F
    0b01101111, // g
    0b01110110, // H
    0b00110000, // i
    0b00011110, // j
    0b01111000, // K
    0b00111000, // L
    0b00110111, // M
    0b01110010, // N
    0b00111111, // 0
    0b01110011, // P
    0b01100111, // Q
    0b01010000, // R
    0b01101101, // S
    0b00110001, // T
    0b00111110, // U
    0b00011100, // W
    0b00111110, // V
    0b01111111, // X
    0b01110010, // Y
    0b01011011 // Z
};


void initLedDisplay();
void displayLEDs(char digit, char value);

void shiftInByte(char byte);
void shiftInBit(char input);

#endif