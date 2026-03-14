#ifndef STATETEST_H
#define STATETEST_H

#include "state.h"

class StateTEST : public State {
    public:
        void enter() override;
        void update() override;
        void exit() override;
};

#endif