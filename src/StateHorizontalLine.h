#ifndef STATEMAZE1_H
#define STATEMAZE1_H

#include "state.h"
#include "StateLineFollow.h"

class StateMaze1 : public StateLineFollow {
    private:
        unsigned int distance = 0;

    public:
        void enter() override;
        void update() override;
        void exit() override;
};

#endif