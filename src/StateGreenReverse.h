#ifndef STATEGREENREVERSE_H
#define STATEGREENREVERSE_H

#include "StateRedReverse.h"

class StateGreenReverse : public StateRedReverse {

    protected:
        unsigned int color = 4;
};

#endif