#ifndef STATEBlUE_H
#define STATEBLUE_H

#include "StateColor.h"

class StateBlue : public StateColor {
    protected:
        unsigned int color = 2;
    public:
        void update() override;

};

#endif