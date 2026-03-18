#ifndef STATEBARRED_H
#define STATEBARRED_H

#include "state.h"
#include "StateBar.h"

class StateBarRed : public StateBar {
    public:
        void update() override;
};

#endif