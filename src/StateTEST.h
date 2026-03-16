#ifndef STATETEST_H
#define STATETEST_H

#include "state.h"

class StateTEST : public State {
    private:
        long distance = 0;
        long angle = 0;
        int numMeas = 0;

        static const int kp = 8;
        static const int kd = 1;
        short ep = 0; // error proportional
        short ed = 0; // error derivative
        short currentVfwd = 100, currentVhorz = 0, currentOmega = 0;
        static const short vfwdSetpoint = 150, vhorzSetpoint = 0, omegaSetpoint;

        unsigned long lastUpdate;
        unsigned long newUpdate;

        unsigned long startTime;
        unsigned long currentTime;
        unsigned long moveTime = 2000000; // 2 second (in microseconds)


    public:
        void enter() override;
        void update() override;
        void exit() override;
};

#endif