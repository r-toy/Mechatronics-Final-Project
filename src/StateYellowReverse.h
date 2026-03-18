#ifndef STATEYELLOWREVERSE_H
#define STATEYELLOWREVERSE_H

#include "StateRedReverse.h"

class StateYellowReverse : public StateRedReverse {

    protected:
        unsigned int color = 1;
};

#endif