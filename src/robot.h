#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include <QTRSensors.h>

void leftISR();
void rightISR();

class Robot {

    private:
        const unsigned short enableRight = 11;
        const unsigned short enableLeft = 12; 
        const unsigned short in1Right = 30;
        const unsigned short in2Right = 31;
        const unsigned short in1Left = 32;
        const unsigned short in2Left = 33;
        const unsigned short pushbutton = 27;
        const unsigned short pinLeftA = 2;
        const unsigned short pinLeftB = 22;
        const unsigned short pinRightA = 3;
        const unsigned short pinRightB = 23;
        const unsigned short LEDbase = 36;
        const unsigned short width = 240; //mm
        const unsigned long nmPerCount = 226892;

        QTRSensors qtr;
        static const unsigned char SensorCount = 8;
        const unsigned short EmitterPin = 45;
        uint16_t sensorValues[SensorCount];
        unsigned short sensorMax[SensorCount];
        unsigned short sensorMin[SensorCount];
        unsigned short sensorAvg[SensorCount];
        unsigned short sensorPos;

        volatile int countLeft_v = 0;
        volatile int countRight_v = 0;

    public:
        Robot();
        ~Robot() = default;

        int pushbuttonRead();
        void calibrateLineSensor();
        int measureLine();
        void measureSpeed();
        void diffDrive(short, short);

        void LeftDistance();
        void RightDistance();

        short measuredSpeed;
        short measuredBalance;

        const unsigned long timestep = 50000;

};

#endif