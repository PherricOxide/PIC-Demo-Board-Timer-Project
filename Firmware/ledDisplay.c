#include<htc.h>
#include "ledDisplay.h"

#define SHIFT_DATA RD6
#define SHIFT_CLOCK RD5
#define SHIFT_STROBE RD7
#define DIGIT0 RC2
#define DIGIT1 RD0
#define DIGIT2 RD1
#define DIGIT3 RD4


void initLedDisplay() {
    // Digit selections
    TRISC2 = 0;
    TRISD0 = 0;
    TRISD1 = 0;
    TRISD4 = 0;

    // Shift reg clock
    TRISD5 = 0;

    // Shift reg data
    TRISD6 = 0;

    // Shift reg strobe
    TRISD7 = 0;

    SHIFT_STROBE = 0;
    SHIFT_CLOCK = 0;
}

void displayLEDs(char digit, char value) {
    if (value != ' ' && (value < 'A' || value > 'Z'))
        return;

    DIGIT0 = DIGIT1 = DIGIT2 = DIGIT3 = 1;

    if (value == ' ') {
        shiftInByte(0);
    } else {
        shiftInByte(alpha[value - 65]);
    }
    SHIFT_STROBE = 1;
    SHIFT_STROBE = 0;

    if (digit == 0) {
        DIGIT0 = 0;
    } else if (digit == 1) {
        DIGIT1 = 0;
    } else if (digit == 2) {
        DIGIT2 = 0;
    } else if (digit == 3) {
        DIGIT3 = 0;
    }
}

void shiftInBit(char input) {
    if (input == 0) {
        SHIFT_DATA = 0;
    } else {
        SHIFT_DATA = 1;
    }
    
    SHIFT_CLOCK = 1;
    SHIFT_CLOCK = 0;
}

void shiftInByte(char byte) {
    for (char i = 0; i < 8; i++) {
        shiftInBit((1 << i) & byte);
    }
}