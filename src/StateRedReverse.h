#ifndef STATEREDREVERSE_H
#define STATEREDREVERSE_H

#include "StateRed.h"

class StateRedReverse : public StateRed {
    
    public:
        void update() override;
        void exit() override;
};

#endif