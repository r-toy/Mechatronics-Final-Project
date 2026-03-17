#ifndef STATEDRIFT_H
#define STATEDRIFT_H

#include "state.h"

class StateDrift : public State {
    
    public:
        void enter() override;
        void update() override;
        void exit() override;
};

#endif