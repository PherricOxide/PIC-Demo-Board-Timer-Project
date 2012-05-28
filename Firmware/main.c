// main.c

/*********************************************************************
* Software License Agreement:
*
* The software supplied herewith by Microchip Technology Incorporated
* (the "Company") for its PICmicro® Microcontroller is intended and
* supplied to you, the Company's customer, for use solely and
* exclusively on Microchip PICmicro Microcontroller products. The
* software is owned by the Company and/or its supplier, and is
* protected under applicable copyright laws. All rights are reserved.
* Any use in violation of the foregoing restrictions may subject the
* user to criminal sanctions under applicable laws, as well as to
* civil liability for the breach of the terms and conditions of this
* license.
*
* THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
* WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
* TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
* CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
*********************************************************************/
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

enum stateType
{
    COUNTING,
    SITTING,
    
    SETTING_DIGIT0,
    SETTING_DIGIT1,
    SETTING_DIGIT2,
    SETTING_DIGIT3
};

void main ()
{
     // Default timer value to 10 minutes
    int timerValue = 600;
   
    int count = timerValue;
 
    enum stateType state = SETTING_DIGIT0;

    // This is used for the blinking during set mode
    char currentDigitValue = 0;

    lcd_init();
    initTimer();
    initButtons();
    initEncoder();

    displayTime(timerValue);

    while (1)
    {
        enum button buttonState = getButtonState();
        enum encoderChange encoderState = getEncoderState();
        if (state == SITTING || state == COUNTING)
        {
            // Enable set mode
            if (buttonState == BUTTON_S2)
            {
                count = 0;
                timerValue = 0;
                currentDigitValue = 0;
                state = SETTING_DIGIT0;
                
                bcd.digit0 = 0xA;
                bcd.digit1 = 0xA;
                bcd.digit2 = 0xA;
                bcd.digit3 = 0xA;
                lcd_display_digits(bcd);
            }
            // Reset the timer
            else if (buttonState == BUTTON_S3)
            {
                count = timerValue;
                state = SITTING;
                displayTime(timerValue);
            }

            // Start the counter
            else if (state == SITTING && buttonState == BUTTON_S4)
            {
                count = timerValue;
                state = COUNTING;
                displayTime(count);
            }
        }
        else if (state == SETTING_DIGIT0
                || state == SETTING_DIGIT1
                || state == SETTING_DIGIT2
                || state == SETTING_DIGIT3)
        {
            if (buttonState == BUTTON_S2)
            {
                switch (state)
                {
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
            }
            else if (buttonState == BUTTON_S4 || encoderState == STEPFORWARD)
            {
                if (currentDigitValue == 9)
                    currentDigitValue = 0;
                else
                    currentDigitValue++;
            }
            else if (buttonState == BUTTON_S3 || encoderState == STEPBACKWARD)
            {   
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

        if (TMR1IF) 
        {
            switch (state)
            {
                case COUNTING:
                    count--;
                    displayTime(count);
                    
                    if (count == 0)
                    {
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

void displayTime(int seconds)
{
    bcd.digit0 = (seconds % 60) % 10;
    bcd.digit1 = (seconds % 60) / 10;
    bcd.digit2 = (seconds / 60) % 10;
    bcd.digit3 = (seconds / 60) / 10;
    lcd_display_digits(bcd);
}

void initTimer()
{
    // Set 2mhz system clock
    //IRCF0 = 1;
    //IRCF1 = 0;
    //IRCF2 = 1;

    // Disable timer
    TMR1ON = 0;
    
    // Set internal clock
    TMR1CS = 0;
    
    T1OSCEN = 0;
    T1SYNC = 1;
    // Always count desite gate value
    TMR1GE = 0;
    // Set 1:8 prescalor
    //T1CKPS0 = 1;
    //T1CKPS1 = 1;

    // Clear overflow flag and timer
    TMR1IF = 0;
    TMR1 = 0x0BDB;

    // Enable timer1
    TMR1ON = 1;
}
