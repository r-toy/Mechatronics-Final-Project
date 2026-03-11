#include "robot.h"
#include "mymacros.h"

Robot::Robot() {
    Serial.begin(9600);

    qtr.setTypeRC();
    qtr.setSensorPins((const uint8_t[8]){46, 47, 48, 49, 50, 51, 52, 53}, SensorCount);
    qtr.setEmitterPin(EmitterPin);

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(pushbutton,INPUT_PULLUP);
    pinMode(enableLeft, OUTPUT);
    pinMode(enableRight, OUTPUT);
    pinMode(in1Left, OUTPUT);
    pinMode(in1Right, OUTPUT);
    pinMode(in2Left, OUTPUT);
    pinMode(in2Right, OUTPUT);
    pinMode(pinLeftA, INPUT);
    pinMode(pinLeftB, INPUT);
    pinMode(pinRightA, INPUT);
    pinMode(pinRightB, INPUT);

    for (int i = 0; i < SensorCount; i++) {
        pinMode(36+i, OUTPUT);
    }

    digitalWrite(enableLeft,LOW);
    digitalWrite(enableRight,LOW);

    attachInterrupt(digitalPinToInterrupt(pinLeftA),leftISR,RISING);
    attachInterrupt(digitalPinToInterrupt(pinRightA),rightISR,RISING);

    calibrateLineSensor();
}

void Robot::LeftDistance() {
    if(digitalRead(pinLeftB) == 1)
        countLeft_v++;
    else
        countLeft_v--;
    return;
}

void Robot::RightDistance() {
    if(digitalRead(pinRightB) == 1)
        countRight_v--;
    else
        countRight_v++;
    return;
}

int Robot::pushbuttonRead() {
    int pressedornot = !digitalRead(pushbutton);
    return pressedornot;
}

void Robot::calibrateLineSensor() {
    // black calibration
    while (digitalRead(pushbutton) == 1) {
        // wait
    }
    digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode
    for (uint16_t i = 0; i < 100; i++)
    {
        qtr.calibrate();
    }
    digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate first calibration finished

    // white calibation
    while (digitalRead(pushbutton) == 1) {
        // wait
    }
    digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode
    for (uint16_t i = 0; i < 100; i++)
    {
        qtr.calibrate();
    }
    digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration

    for (uint8_t i = 0; i < SensorCount; i++) {
        sensorAvg[i] = (sensorMax[i] + sensorMin[i]) / 2;
    }
}

int Robot::measureLine() {
    sensorPos = qtr.readLineBlack(sensorValues);
    Serial.println(sensorPos);
    short error = -24 + (48 * (long)(sensorPos) / 7000);

    for (int i = 0; i < SensorCount; i++) {
      if (sensorValues[i] > sensorAvg[i]) {
        digitalWrite(LEDbase+i,LOW);
      }
      else {
        digitalWrite(LEDbase+i,HIGH);
      }
    }

    return error;
}

void Robot::measureSpeed() {
  noInterrupts();
    long countLeft = countLeft_v;
    long countRight = countRight_v;
    countLeft_v = 0;
    countRight_v = 0;
    interrupts();

    // 453.786 micrometers per count
    // divided by microseconds
    // reduce to nm per count to end up with mm / sec

    long measLeftSpeed = 453786 * countLeft / timestep;
    long measRightSpeed = 453786 * countRight / timestep;
    measuredSpeed = (measLeftSpeed + measRightSpeed) / 2; //mm per second
    measuredBalance = 1024 * (measLeftSpeed - measRightSpeed) / width; // 1024th of radians per second
    return; 
}

void Robot::diffDrive(short vcm, short omega) {
    digitalWrite(enableRight, LOW);
    digitalWrite(enableLeft, LOW);
    int16_t vR = vcm + (width / 2 * omega) / 64;
    int16_t vL = vcm - (width / 2 * omega) / 64;

    unsigned short dirL, dirR;
    dirL = SIGN(vL);
    dirR = SIGN(vR);
    vL = ABS(vL);
    vR = ABS(vR);

    unsigned short pwmR, pwmL;
    pwmL = (vL > 255) ? 255 : vL;
    pwmR = (vR > 255) ? 255 : vR;
    
    digitalWrite(in1Left, dirL);
    digitalWrite(in2Left, !dirL);
    digitalWrite(in1Right, dirR);
    digitalWrite(in2Right, !dirR);

    analogWrite(enableRight, pwmR);
    analogWrite(enableLeft, pwmL);

    return;
}