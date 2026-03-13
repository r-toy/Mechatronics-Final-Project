#include "robot.h"
#include "mymacros.h"

void isrFLDistance();
void isrFRDistance();
void isrBLDistance();
void isrBRDistance();

Robot::Robot() {
    Serial.begin(9600);

    qtr.setTypeRC();
    qtr.setSensorPins((const uint8_t[8]){46, 47, 48, 49, 50, 51, 52, 53}, SensorCount);
    qtr.setEmitterPin(EmitterPin);

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(pushbutton,INPUT_PULLUP);

    pinMode(enableFL,OUTPUT);
    pinMode(enableFR,OUTPUT);

    pinMode(enableBL,OUTPUT);
    pinMode(enableBR,OUTPUT);

    pinMode(in1FL,OUTPUT);
    pinMode(in2FL,OUTPUT);
    pinMode(in1FR,OUTPUT);
    pinMode(in2FR,OUTPUT);

    pinMode(in1BL,OUTPUT);
    pinMode(in2BL,OUTPUT);
    pinMode(in1BR,OUTPUT);
    pinMode(in2BR,OUTPUT);

    pinMode(encFL_A,INPUT);
    pinMode(encFL_B,INPUT);
    pinMode(encFR_A,INPUT);
    pinMode(encFR_B,INPUT);
    
    pinMode(encBL_A,INPUT);
    pinMode(encBL_B,INPUT);
    pinMode(encBR_A,INPUT);
    pinMode(encBR_B,INPUT);

    attachInterrupt(digitalPinToInterrupt(encFL_A),isrFLDistance,RISING);
    attachInterrupt(digitalPinToInterrupt(encFR_A),isrFRDistance,RISING);
    attachInterrupt(digitalPinToInterrupt(encBL_A),isrBLDistance,RISING);
    attachInterrupt(digitalPinToInterrupt(encBR_A),isrBRDistance,RISING);

    for (int i = 0; i < SensorCount; i++) {
        pinMode(36+i, OUTPUT);
    }

    digitalWrite(enableFL,LOW);
    digitalWrite(enableFR,LOW);
    digitalWrite(enableBL,LOW);
    digitalWrite(enableBR,LOW);

    calibrateLineSensor();
}

/*
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
    */

void Robot::FLDistance() {
  if(digitalRead(encFL_B) == 1)
    countFL_v++;
  else
    countFL_v--;
}

void Robot::FRDistance() {
  if(digitalRead(encFR_B) == 0)
    countFR_v++;
  else
    countFR_v--;
}

void Robot::BLDistance() {
  if(digitalRead(encBL_B) == 1)
    countBL_v++;
  else
    countBL_v--;
}

void Robot::BRDistance() {
  if(digitalRead(encBR_B) == 0)
    countBR_v++;
  else
    countBR_v--;
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
    short error = 24 - (48 * (long)(sensorPos) / 7000);

    for (int i = 0; i < SensorCount; i++) {
      if (sensorValues[i] > sensorAvg[i]) {
        // digitalWrite(LEDbase+i,LOW);
        blacks++;
      }
    //   else {
    //     digitalWrite(LEDbase+i,HIGH);
    //   }
    }

    return error;
}

void Robot::measureSpeed() {
    noInterrupts();
    long countFL = countFL_v;
    long countFR = countFR_v;
    long countBL = countBL_v;
    long countBR = countBR_v;
    countFL_v = 0;
    countFR_v = 0;
    countBL_v = 0;
    countBR_v = 0;
    interrupts();

    // Serial.print("countLeft = "); Serial.print(countLeft);
    // Serial.print(" countRight = "); Serial.println(countRight);

    // 167.552 micrometers per count
    // divided by microseconds
    // reduce to nm per count to end up with mm / sec
    
    long measFLspeed = umPerCt * countFL / timestep;
    long measFRspeed = umPerCt * countFR / timestep;
    long measBLspeed = umPerCt * countBL / timestep;
    long measBRspeed = umPerCt * countBR / timestep;

    // baking in the sqrt(2) factor: 128 * (sum) / (128 * 4 * sqrt(2))
    measuredvFwd  = 128 * (measFLspeed + measFRspeed + measBLspeed + measBRspeed) / 722;
    measuredvHorz = 128 * (measFLspeed - measFRspeed - measBLspeed + measBRspeed) / 722;
    measuredOmega = 256 * (measFRspeed - measFLspeed - measBLspeed + measBRspeed) / centercorner;

}

void Robot::omni4WD(long vfwd, long vhorz, long omega) {
    // baking in the sqrt(2) factor: sqrt(2) * 256 * (45deg rotation) / 256
    long vFL = 361 * (vfwd + vhorz) / 256 - (centercorner * omega) / 64;
    long vFR = 361 * (vfwd - vhorz) / 256 + (centercorner * omega) / 64;
    long vBL = 361 * (vfwd - vhorz) / 256 - (centercorner * omega) / 64;
    long vBR = 361 * (vfwd + vhorz) / 256 + (centercorner * omega) / 64;

    /*
    Serial.print(" vFL = "); Serial.print(vFL);
    Serial.print(" vFR = "); Serial.print(vFR);
    Serial.print(" vBL = "); Serial.print(vBL);
    Serial.print(" vBR = "); Serial.println(vBR);
    //*/
    
    uint16_t dirFL, dirFR, dirBL, dirBR;
    dirFL = SIGN(vFL);
    dirFR = SIGN(vFR);
    dirBL = SIGN(vBL);
    dirBR = SIGN(vBR);
    vFL = ABS(vFL);
    vFR = ABS(vFR);
    vBL = ABS(vBL);
    vBR = ABS(vBR);


    uint16_t pwmFL, pwmFR, pwmBL, pwmBR;

    pwmFL = (vFL > 255) ? 255 : vFL;
    pwmFR = (vFR > 255) ? 255 : vFR;
    pwmBL = (vBL > 255) ? 255 : vBL;
    pwmBR = (vBR > 255) ? 255 : vBR;
    
    digitalWrite(enableFL, LOW);
    digitalWrite(enableFR, LOW);
    digitalWrite(enableBL, LOW);
    digitalWrite(enableBR, LOW);

    digitalWrite(in1FL, dirFL);
    digitalWrite(in2FL, !dirFL);
    digitalWrite(in1FR, dirFR);
    digitalWrite(in2FR, !dirFR);
    digitalWrite(in1BL, dirBL);
    digitalWrite(in2BL, !dirBL);
    digitalWrite(in1BR, dirBR);
    digitalWrite(in2BR, !dirBR);
    // Serial.print(pwmL);
    // Serial.print(" ");
    // Serial.println(pwmR);

    analogWrite(enableFL, pwmFL);
    analogWrite(enableFR, pwmFR);
    analogWrite(enableBL, pwmBL);
    analogWrite(enableBR, pwmBR);

    return;
}

void Robot::rightTurn(short vfwd) {
    omni4WD(vfwd, 0, -64);
    delay(1000);

    return;
}

void Robot::leftTurn(short vfwd) {
    omni4WD(vfwd, 0, 64);
    delay(1000);

    return;
}

void Robot::resetBlacks() {
    blacks = 0;

    return;
}

int Robot::readBlacks() {
    return blacks;
}


/*
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
    short vR = vcm + (width / 2 * omega) / 64;
    short vL = vcm - (width / 2 * omega) / 64;

    // Serial.print("VR "); Serial.print(vR); Serial.print(" VL "); Serial.println(vL);
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
*/
