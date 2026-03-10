#ifndef STATEINIT_H
#define STATEINIT_H

#include "state.h"

class StateInit : public State {
    public:
        void enter() override;
        void update() override;
        void exit() override;
};

#endif