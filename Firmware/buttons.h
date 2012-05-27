#ifndef BUTTONS_H
#define BUTTONS_H

#include <htc.h>

enum button
{
    NONE,
    BUTTON_S2,
    BUTTON_S3,
    BUTTON_S4
};


enum button getButtonState();
void initButtons();

#endif