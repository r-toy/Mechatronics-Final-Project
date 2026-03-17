#ifndef STATEBUTTONREAD_H
#define STATEBUTTONREAD_H

#include "state.h"
#include "StateLineFollow.h"

class StateButtonRead : public StateLineFollow {
    private:
        unsigned int distance = 0;
        const int redAvg[3] = {1620, 940, 827};
        const int redAvg2[3] = {1059, 208, 180};
        
        const int greenAvg[3] = {984, 1153, 797};
        const int greenAvg2[3] = {335, 912, 371};

        const int blueAvg[3] = {899, 942, 982};
        const int blueAvg2[3] = {296, 670, 1545};

        const int yellowAvg[3] = {736, 558, 280};

        const int blackAvg[3] = {853, 795, 575};
        const int whiteAvg[3] = {1853, 1569, 1597};

    public:
        void enter() override;
        void update() override;
        void exit() override;
};

#endif