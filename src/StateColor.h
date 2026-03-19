#ifndef STATECOLOR_H
#define STATECOLOR_H

#include "state.h"
#include "StateLineFollow.h"

class StateColor : public StateLineFollow {
    protected:
        unsigned int distance = 0;
        unsigned int color = 0;

    public:
        void enter() override;
        void update() override;
        void exit() override;
};

#endif