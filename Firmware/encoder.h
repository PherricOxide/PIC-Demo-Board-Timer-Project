#ifndef ENCODER_H
#define ENCODER_H

#include <htc.h>

enum encoderChange
{
    NOCHANGE,
    STEPFORWARD,
    STEPBACKWARD
};


enum encoderChange getEncoderState();
void initEncoder();

#endif
