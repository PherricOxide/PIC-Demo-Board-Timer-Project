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
#include <htc.h>
#include "Hardware.h"
#include "lcd2.h"
#include "seven_seg.h"


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
BCD_TYPE    bcd;



void main ()
{
    int count = 0;

    lcd_init();
    bcd.digit0 = 0;
    bcd.digit1 = 0;
    bcd.digit2 = 0;
    bcd.digit3 = 0;
    lcd_display_digits(bcd);

    initTimer();

    TRISB7 = 1;
    TRISB6 = 1;

    while (1){
        if (TMR1IF) {
            count++;
            bcd.digit0 = count % 10;
            bcd.digit1 = count / 10;
            bcd.digit2 = count / 60;
            lcd_display_digits(bcd);
            
            
            TMR1IF = 0;
        }

        if (!RB6 && !RB7)
        {
            count = 0;
        } else if (!RB6)
        {

        };

    } 
}

void initTimer()
{
    // Set 2mhz system clock
    IRCF0 = 1;
    IRCF1 = 0;
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
