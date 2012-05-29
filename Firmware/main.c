#include "ledDisplay.h"
#include "seven_seg.h"
#include "Hardware.h"
#include "buttons.h"
#include "encoder.h"
#include "lcd2.h"

#include <htc.h>

__CONFIG(FOSC_INTOSCCLK
        & WDTE_OFF
        & PWRTE_OFF
        & MCLRE_ON
        & CP_OFF
        & CPD_OFF
        & BOREN_ON
        & IESO_ON
        & FCMEN_ON);

void initTimer();
void displayTime(int seconds);

BCD_TYPE    bcd;

char displayString[] = "POST SQQ   ";
int displayStringIndex = 0;

enum stateType {
    COUNTING,
    SITTING,
    
    SETTING_DIGIT0,
    SETTING_DIGIT1,
    SETTING_DIGIT2,
    SETTING_DIGIT3
};

void main () {
    // Current timer state
    enum stateType state = COUNTING;

    // Default timer value to 10 minutes
    int timerValue = 600;

    // Note that count = 4x number of seconds
    int count = timerValue << 2;
 
    // This is used for the blinking digit during set mode
    char currentDigitValue = 0;

    lcd_init();

    initTimer();
    initButtons();
    initEncoder();
    initLedDisplay();
    
    displayTime(timerValue);

    while (1)
    {
        // display the LED string
        displayLEDs(1, displayString[displayStringIndex]);
        displayLEDs(2, displayString[displayStringIndex + 1]);
        displayLEDs(3, displayString[displayStringIndex + 2]);

        // Get user input (buttons and encoder)
        enum button buttonState = getButtonState();
        enum encoderChange encoderState = getEncoderState();

        if (state == SITTING || state == COUNTING) {
            // User wants to enable set mode
            if (buttonState == BUTTON_S2) {
                count = 0;
                timerValue = 0;
                currentDigitValue = 0;
                state = SETTING_DIGIT0;
                
                bcd.digit0 = 0xA;
                bcd.digit1 = 0xA;
                bcd.digit2 = 0xA;
                bcd.digit3 = 0xA;
                lcd_display_digits(bcd);

            // User wants to reset the timer
            } else if (buttonState == BUTTON_S3) {
                count = timerValue << 2;
                state = SITTING;
                displayTime(timerValue);

            // User wants to start the timer
            } else if (state == SITTING && buttonState == BUTTON_S4) {
                count = timerValue << 2;
                state = COUNTING;
                displayTime(count >> 2);
            }
        }

        // If we're already in set mode
        else if (state == SETTING_DIGIT0
                || state == SETTING_DIGIT1
                || state == SETTING_DIGIT2
                || state == SETTING_DIGIT3) {
            // User wants to save digit and go to next
            if (buttonState == BUTTON_S2) {
                switch (state) {
                    case SETTING_DIGIT0:
                    {
                        timerValue += currentDigitValue;
                        currentDigitValue = 0;
                        state = SETTING_DIGIT1;
                        break;
                    }
                    case SETTING_DIGIT1:
                    {
                        timerValue += 10*currentDigitValue;
                        currentDigitValue = 0;
                        state = SETTING_DIGIT2;
                        break;
                    }
                    case SETTING_DIGIT2:
                    {
                        timerValue += 60*currentDigitValue;
                        currentDigitValue = 0;
                        state = SETTING_DIGIT3;
                        break;
                    }
                    case SETTING_DIGIT3:
                    {
                        timerValue += 10*60*currentDigitValue;
                        currentDigitValue = 0;
                        state = SITTING;
                        break;
                    }
                }
            // User wants to increment digit
            } else if (buttonState == BUTTON_S4 || encoderState == STEPFORWARD) {
                if (currentDigitValue == 9)
                    currentDigitValue = 0;
                else
                    currentDigitValue++;
            } else if (buttonState == BUTTON_S3 || encoderState == STEPBACKWARD) {
                if (currentDigitValue == 0)
                    currentDigitValue = 9;
                else
                    currentDigitValue --;
            }
        
            // Update the digit being set on the display
            switch(state)
            {
                case SETTING_DIGIT0:
                    bcd.digit0 = currentDigitValue;
                    break;
                case SETTING_DIGIT1:
                    bcd.digit1 = currentDigitValue;
                    break;
                case SETTING_DIGIT2:
                    bcd.digit2 = currentDigitValue;
                    break;
                case SETTING_DIGIT3:
                    bcd.digit3 = currentDigitValue;
                    break;
            }
            lcd_display_digits(bcd);

        }

        if (TMR1IF) {
            displayStringIndex++;
            if (displayStringIndex >= sizeof(displayString) - 3) {
                displayStringIndex = 0;
            }

            switch (state) {
                case COUNTING:
                    count--;
                    displayTime(count >> 2);
                    
                    if (count == 0) {
                        state = SITTING;
                    }
                    
                    break;
                    
                case SITTING:
                    displayTime(timerValue);
                    break;

                default:
                    break;
                }
            TMR1IF = 0;
        }
    }
}

void displayTime(int seconds) {
    bcd.digit0 = (seconds % 60) % 10;
    bcd.digit1 = (seconds % 60) / 10;
    bcd.digit2 = (seconds / 60) % 10;
    bcd.digit3 = (seconds / 60) / 10;
    lcd_display_digits(bcd);
}

void initTimer() {
    // Set 8mhz system clock
    IRCF0 = 1;
    IRCF1 = 1;
    IRCF2 = 1;

    // Disable timer
    TMR1ON = 0;
    
    // Set internal clock
    TMR1CS = 0;
    
    T1OSCEN = 0;
    T1SYNC = 1;
    // Always count desite gate value
    TMR1GE = 0;
    // Set 1:8 prescalor
    T1CKPS0 = 1;
    T1CKPS1 = 1;

    // Clear overflow flag and timer
    TMR1IF = 0;
    TMR1 = 0x0BDB;

    // Enable timer1
    TMR1ON = 1;
}
