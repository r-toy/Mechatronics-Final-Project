#ifndef STATEMAZE3_H
#define STATEMAZE3_H

#include "state.h"
#include "StateLineFollow.h"

class StateMaze3 : public StateLineFollow {
    private:
        unsigned int distance = 0;
        unsigned long time = 0;
        const unsigned long timeConst = 3000000;

    public:
        void enter() override;
        void update() override;
        void exit() override;
};

#endif