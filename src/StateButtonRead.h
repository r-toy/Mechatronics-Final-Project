#ifndef STATEBUTTONREAD_H
#define STATEBUTTONREAD_H

#include "state.h"
#include <math.h>
#include "StateLineFollow.h"

class StateButtonRead : public StateLineFollow {
    private:
        long cosineSim(long *one, long *two);

        const long redRef[4] = {898,166,132,798};

        double redMagnitude256 = sqrt((double)(redRef[0]*redRef[0] + redRef[1]*redRef[1] + redRef[2]*redRef[2]));
        long redRef2[4] = {redRef[0] * 256 / redMagnitude256, redRef[1] * 256 / redMagnitude256, redRef[2] * 256 / redMagnitude256, redRef[3] * 256 / redMagnitude256};
        const long greenRef[4] = {272,562,230,540};
        double greenMagnitude256 = sqrt((double)(greenRef[0]*greenRef[0] + greenRef[1]*greenRef[1] + greenRef[2]*greenRef[2]));
        long greenRef2[4] = {greenRef[0] * 256 / greenMagnitude256, greenRef[1] * 256 / greenMagnitude256, greenRef[2] * 256 / greenMagnitude256, greenRef[3] * 256 / greenMagnitude256};
        const long blueRef[4] = {231,316,668,798};
        double blueMagnitude256 = sqrt((double)(blueRef[0]*blueRef[0] + blueRef[1]*blueRef[1] + blueRef[2]*blueRef[2]));
        long blueRef2[4] = {blueRef[0] * 256 / blueMagnitude256, blueRef[1] * 256 / blueMagnitude256, blueRef[2] * 256 / blueMagnitude256, blueRef[3] * 256 / blueMagnitude256};
        const long yellowRef[4] = {271,237,108,798};
        double yellowMagnitude256 = sqrt((double)(yellowRef[0]*yellowRef[0] + yellowRef[1]*yellowRef[1] + yellowRef[2]*yellowRef[2]));
        long yellowRef2[4] = {yellowRef[0] * 256 / yellowMagnitude256, yellowRef[1] * 256 / yellowMagnitude256, yellowRef[2] * 256 / yellowMagnitude256, yellowRef[3] * 256 / yellowMagnitude256};

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