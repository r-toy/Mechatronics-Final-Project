#ifndef STATERED_H
#define STATERED_H

#include "StateColor.h"

class StateRed : public StateColor {
    protected:
        unsigned int color = 3;

    public:
        void update() override;

};

#endif