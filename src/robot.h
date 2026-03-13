#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include <QTRSensors.h>

void leftISR();
void rightISR();

class Robot {

    private:

        const int enableFR = 11, enableFL = 12, enableBR = 9, enableBL = 10;
        const int in1FR = 30, in2FR = 31, in1FL = 32, in2FL = 33, in1BR = 36, in2BR = 37, in1BL = 38, in2BL = 39;
        const int encFL_A = 2, encFR_A = 3, encBL_A = 18, encBR_A = 19;
        const int encFL_B = 22, encFR_B = 23, encBL_B = 24, encBR_B = 25;

        const int pushbutton = 26;

        const unsigned short LEDbase = 36;

        int centercorner = 170; //mm, sqrt(2) * width
        long umPerCt = 167552; // micrometers per count, calculate this
        int measuredvFwd = 0, measuredvHorz = 0, measuredOmega = 0;

        QTRSensors qtr;
        static const unsigned char SensorCount = 8;
        const unsigned short EmitterPin = 45;
        uint16_t sensorValues[SensorCount];
        unsigned short sensorMax[SensorCount];
        unsigned short sensorMin[SensorCount];
        unsigned short sensorAvg[SensorCount];
        unsigned short sensorPos;

        volatile int countFL_v;
        volatile int countFR_v;
        volatile int countBL_v;
        volatile int countBR_v;

    public:
        Robot();
        ~Robot() = default;

        int pushbuttonRead();
        void calibrateLineSensor();
        int measureLine();
        void measureSpeed();
        void omni4WD(long, long, long);

        void FLDistance();
        void FRDistance();
        void BLDistance();
        void BRDistance();

        short measuredSpeed;
        short measuredBalance;

        const unsigned long timestep = 50000;

};

#endif