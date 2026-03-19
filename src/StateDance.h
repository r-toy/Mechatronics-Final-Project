#ifndef STATEDANCE_H
#define STATEDANCE_H

#include "State.h"
#include "robot.h"

class StateDance : public State{
    private:
        unsigned long lastUpdate, newUpdate;
        long measrot = 0;
        long phase = 512; // start at the min of myCos()
        long omega = 64;
        long vYset = 150;

    public:
        void enter() override;
        void update() override;
        void exit() override;
};

#endif