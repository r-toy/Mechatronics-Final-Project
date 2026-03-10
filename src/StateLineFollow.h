#ifndef STATELINEFOLLOW_H
#define STATELINEFOLLOW_H

#include "state.h"

class StateLineFollow : public State {
    private:
        const int kp = 8;
        const int kd = 1;
        short ep = 0; // error proportional
        short ed = 0; // error derivative
        short currentBalance = 0;
        short speedSetpoint = 200;

        unsigned long lastUpdate;
        unsigned long newUpdate;

    public:
        void enter() override;
        void update() override;
        void exit() override;
};

#endif