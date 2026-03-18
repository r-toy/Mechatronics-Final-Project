#ifndef STATEGREEN_H
#define STATEGREEN_H

#include "StateColor.h"

class StateGreen : public StateColor {
    protected:
        unsigned int color = 4;
    public:
        void update() override;

};

#endif