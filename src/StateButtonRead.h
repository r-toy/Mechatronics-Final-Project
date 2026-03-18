#ifndef STATEBUTTONREAD_H
#define STATEBUTTONREAD_H

#include "state.h"
#include "StateLineFollow.h"

class StateButtonRead : public StateLineFollow {
    private:
        float cosineSim(int *one, int *two);

        const int redRef[4] = {0,0,0,0};
        const int greenRef[4] = {0,0,0,0};
        const int blueRef[4] = {0,0,0,0};
        const int yellowRef[4] = {0,0,0,0};

        unsigned int distance = 0;
        int redAvg[3] = {1620, 940, 827};
        int redAvg2[3] = {1059, 208, 180};
        
        int greenAvg[3] = {984, 1153, 797};
        int greenAvg2[3] = {335, 912, 371};

        int blueAvg[3] = {899, 942, 982};
        int blueAvg2[3] = {296, 670, 1545};

        int yellowAvg[3] = {736, 558, 280};

        int blackAvg[3] = {853, 795, 575};
        int whiteAvg[3] = {1853, 1569, 1597};

    public:
        void enter() override;
        void update() override;
        void exit() override;
};

#endif