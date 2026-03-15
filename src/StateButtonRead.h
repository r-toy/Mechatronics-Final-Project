#ifndef STATEBUTTONREAD_H
#define STATEBUTTONREAD_H

#include "state.h"
#include "StateLineFollow.h"

class StateButtonRead : public StateLineFollow {
    private:
        unsigned int distance = 0;

    public:
        void enter() override;
        void update() override;
        void exit() override;
};

#endif