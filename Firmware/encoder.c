#include "encoder.h"

#define PHASE1 0
#define PHASE2 1
#define PHASE3 3
#define PHASE4 2

enum encoderChange lastState;

void initEncoder()
{
     // Disable the ADC
     ADON = 0;

     // Set the pin back to digital mode,
     // otherwise it will always read 0
     AN6 = 0;
     AN7 = 0;

     // Set the buttons to input mode
    TRISE1 = 1;
    TRISE2 = 1;
}

enum encoderChange getEncoderState()
{
    char currentState = 0 | RE1 | (RE2 << 1);

    enum encoderChange change = NOCHANGE;

    if (currentState != lastState) {
        if (lastState == PHASE1) {
           if (currentState == PHASE2) {
               change = STEPFORWARD;
           } else if (currentState == PHASE4) {
               change = STEPBACKWARD;
           }
        }
    }

    lastState = currentState;
    return change;
}
