#ifndef STATELINEFOLLOW_H
#define STATELINEFOLLOW_H

#include "state.h"

class StateLineFollow : public State {
    protected:
        const int kp = 8;
        const int kd = 1;
        short ep = 0; // error proportional
        short ed = 0; // error derivative
        short currentVfwd = 100, currentVhorz = 0, currentOmega = 0;
        short vfwdSetpoint = 150, vhorzSetpoint = 0, omegaSetpoint;

        unsigned long lastUpdate;
        unsigned long newUpdate;

    public:
        void enter() override;
        void update() override;
        void exit() override;
};

#endif