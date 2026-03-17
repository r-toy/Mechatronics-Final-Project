#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include <QTRSensors.h>
#include <Adafruit_TCS34725.h>
#include <Servo.h>

void leftISR();
void rightISR();

class Robot {

    private:
        int defaultEndcon();

        const int enableFR = 11, enableFL = 12, enableBR = 9, enableBL = 10;
        const int in1FR = 30, in2FR = 31, in1FL = 32, in2FL = 33, in1BR = 36, in2BR = 37, in1BL = 38, in2BL = 39;
        const int encFL_A = 2, encFR_A = 3, encBL_A = 18, encBR_A = 19;
        const int encFL_B = 22, encFR_B = 23, encBL_B = 24, encBR_B = 25;

        const int pushbutton = 26;

        const unsigned short LEDbase = 36;

        // long umPerCt = 167552; //theoretical value based on wheel diameter
        long umPerCt = 335104; // micrometers per count, estimated experimentally (its twice the above value (?) )

        int centercorner = 152; //mm, sqrt(2)/2 * width
        // long rotPermm = 24; // 16,384ths of a rotation (or sixteenth of wolfeians) rotated per mm moved on average by wheels
        long rotPermm = 47; // the experimentally found version of rotPermm (also twice for some reason (?) )

        Adafruit_TCS34725 tcs;
        unsigned short r, g, b, c, colorTemp, lux;
        QTRSensors qtr;
        static const unsigned char SensorCount = 8;
        const unsigned short EmitterPin = 45;
        uint16_t sensorValues[SensorCount];
        unsigned short sensorMax[SensorCount];
        unsigned short sensorMin[SensorCount];
        unsigned short sensorAvg[SensorCount];
        unsigned short sensorPos;

        Servo armServo;
        unsigned short servoPin = 4;

        volatile int countFL_v = 0;
        volatile int countFR_v = 0;
        volatile int countBL_v = 0;
        volatile int countBR_v = 0;

        // default speed to use for move3DOF
        static const int defaultSpeed = 100;
        static const int defaultOmega = 24;

        // 3DOF control gains
        // in order: 
        // ki/kp/kv were 12/8/3 for ok driving
        const long ki = 2, ki_omega = 8; //kio = 8, kpo = 14, kdo = 1 for good turns
        const long kp = 4, kp_omega = 14, kp_omegaOpp = 16;
        const long kd = 0, kd_omega = 1, kd_omegaOpp = 4;

        // lookup
        int mySinTable[1024];
        int myCosTable[1024];

    public:
        Robot();
        ~Robot() = default;

        int pushbuttonRead();
        int readBlackSenses();
        void calibrateLineSensor();
        int measureLine();
        void measureSpeed();
        void senseColor();
        const int *readColors();
        int colorDetect();

        int measuredvFwd = 0, measuredvHorz = 0, measuredOmega = 0, measuredOppW;

        void brake();
        void omni4WD(long vfwd, long vhorz, long Omega, long omegaOpposing = 0);
        void move3DOF_nofdbk(long ydist, long xdist, long rotation, long speed = defaultSpeed, long omega = defaultOmega);
        void move3DOF(long ydist, long xdist, long rotation, int (Robot::*endcon)(void) = &Robot::defaultEndcon, long speed = defaultSpeed);
        void move3DOF_heading(long ydist, long xdist, long rotation, int (Robot::*endcon)(void) = &Robot::defaultEndcon, long speed = defaultSpeed);
        void rightTurn(short);
        void leftTurn(short);
        void servoPosition(int);

        void FLDistance();
        void FRDistance();
        void BLDistance();
        void BRDistance();

        long mySin(long A);
        long myCos(long A);

        // num sensors Black
        unsigned int blackSenses = 0;
        int scanReadSenses();

        // DONT CHANGE THIS ANYMORE
        // baked into a decent amount of the measurement stuff
        const long timestep = 50000;

};

#endif