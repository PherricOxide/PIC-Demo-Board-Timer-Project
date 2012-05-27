#include "buttons.h"

void initButtons()
{
    // Set the buttons to input mode
    TRISB6 = 1;
    TRISB7 = 1;
}

enum button getButtonState()
{
    enum button state = NONE;
    if (!RB6 && !RB7)
    {
        state = BUTTON_S2;
    }
    else if (!RB6)
    {
        state = BUTTON_S3;
        while (!RB6)
            if (!RB7)
                state = BUTTON_S2;
    }
    else if (!RB7)
    {
        state = BUTTON_S4;
        while (!RB7)
            if (!RB6)
                state = BUTTON_S2;
    }

    // Wait for them to show as depressed
    while (!RB6 || !RB7);
    
    return state;
}
