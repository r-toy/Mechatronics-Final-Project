#ifndef STATEREDREVERSE_H
#define STATEREDREVERSE_H

#include "StateRed.h"

class StateReverse : public StateLineFollow {
    private:
        int color = 0;

    public:
        void enter() override;
        void update() override;
        void exit() override;
};

#endif