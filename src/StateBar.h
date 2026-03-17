#ifndef STATEBAR_H
#define STATEBAR_H

#include "state.h"
#include "StateLineFollow.h"

class StateBar : public StateLineFollow {
    private:
        unsigned int distance = 0;

    public:
        void enter() override;
        void update() override;
        void exit() override;
};

#endif