#ifndef STATETESTDRIVE_H
#define STATETESTDRIVE_H

#include "state.h"

class StateTestDrive : public State {
    private:
        short speedSetpoint = 150;

    public:
        void enter() override;
        void update() override;
        void exit() override;
};

#endif