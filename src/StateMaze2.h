#ifndef STATEMAZE2_H
#define STATEMAZE2_H

#include "state.h"
#include "StateLineFollow.h"

class StateMaze2 : public StateLineFollow {
    private:
        unsigned int distance = 0;

    public:
        void enter() override;
        void update() override;
        void exit() override;
};

#endif