#include "robot.h"
#include "mymacros.h"
#include <Adafruit_TCS34725.h>
#include <math.h>

// using 16384 per rotation and 1024 table size
// output scaled -256 to 256
long Robot::mySin(long A) {
    return mySinTable[pos_mod(A,16384)/16];
}

// using 16384 per rotation and 1024 table size
// output scaled -256 to 256
long Robot::myCos(long A) {
    return myCosTable[pos_mod(A,16384)/16];
}

void isrFLDistance();
void isrFRDistance();
void isrBLDistance();
void isrBRDistance();

Robot::Robot() {
    Serial.begin(9600);

    Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
    if (tcs.begin()) {
        Serial.println("Found sensor");
    } else {
        Serial.println("No TCS34725 found ... check your connections");
        while (1);
    }

    qtr.setTypeRC();
    qtr.setSensorPins((const uint8_t[8]){46, 47, 48, 49, 50, 51, 52, 53}, SensorCount);
    qtr.setEmitterPin(EmitterPin);

    armServo.attach(servoPin);
    armServo.write(180);

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

    // initialize sine and cosine lookup tables
    for (int i = 0; i < 1024; i++){
        mySinTable[i] = (int)(256*sin(i*2.0*PI/1024.0));
        myCosTable[i] = (int)(256*cos(i*2.0*PI/1024.0));
    }
}

int Robot::defaultEndcon(){
    return 0;
}

void Robot::FLDistance() {
  if(digitalRead(encFL_B) == 0)
    countFL_v++;
  else
    countFL_v--;
}

void Robot::FRDistance() {
  if(digitalRead(encFR_B) == 1)
    countFR_v++;
  else
    countFR_v--;
}

void Robot::BLDistance() {
  if(digitalRead(encBL_B) == 0)
    countBL_v++;
  else
    countBL_v--;
}

void Robot::BRDistance() {
  if(digitalRead(encBR_B) == 1)
    countBR_v++;
  else
    countBR_v--;
}

int Robot::pushbuttonRead() {
    int pressedornot = !digitalRead(pushbutton);
    return pressedornot;
}

void Robot::calibrateLineSensor() {
    digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode
    for (uint16_t i = 0; i < 100; i++)
    {
        qtr.calibrate();
    }
    digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate first calibration finished

    for (uint8_t i = 0; i < SensorCount; i++) {
        sensorAvg[i] = (sensorMax[i] + sensorMin[i]) / 2;
    }
}

int Robot::measureLine() {
    sensorPos = qtr.readLineBlack(sensorValues);
    // Serial.println(sensorPos);
    short error = 24 - (48 * (long)(sensorPos) / 7000);

    blackSenses = 0;
    for (int i = 0; i < SensorCount; i++) {
        if (sensorValues[i] > blackThreshold) {
            // digitalWrite(LEDbase+i,LOW);
            blackSenses++;
        }
        // Serial.print(sensorValues[i]); Serial.print(" ");
    //   else {
    //     digitalWrite(LEDbase+i,HIGH);
    //   }
    }
    // Serial.println("");

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

    // Serial.print("countFL = "); Serial.print(countFL);
    // Serial.print(" countFR = "); Serial.print(countFR);
    // Serial.print(" countBL = "); Serial.print(countBL);
    // Serial.print(" countBR = "); Serial.print(countBR);

    // 167.552 micrometers per count
    // divided by microseconds
    // reduce to nm per count to end up with mm / sec
    
    long measFLspeed = umPerCt * countFL / timestep;
    long measFRspeed = umPerCt * countFR / timestep;
    long measBLspeed = umPerCt * countBL / timestep;
    long measBRspeed = umPerCt * countBR / timestep;

    // Serial.print(" measFLspeed = "); Serial.print(measFLspeed);
    // Serial.print(" measFRspeed = "); Serial.print(measFRspeed);
    // Serial.print(" measBLspeed = "); Serial.print(measBLspeed);
    // Serial.print(" measBRspeed = "); Serial.print(measBRspeed);

   // baking in the sqrt(2) factor: 128 * (sum) / (128 * 4 * sqrt(2))
    measuredvFwd  = 128 * (measFLspeed + measFRspeed + measBLspeed + measBRspeed) / 722;
    measuredvHorz = 128 * (measFLspeed - measFRspeed - measBLspeed + measBRspeed) / 722;

    // measuredOmega equals average of velocities to the left around center divided by distance to center
    // measuredOmega = 256 * (measFRspeed - measFLspeed - measBLspeed + measBRspeed) / centercorner; // in 1024ths of radians

    measuredOmega = rotPermm * (long)(measFRspeed - measFLspeed - measBLspeed + measBRspeed) / 16; // in 16384th of rotation/sec

    measuredOppW = 256 * (measFLspeed + measFRspeed - measBLspeed - measBRspeed) / centercorner; // measured self-opposing angular speed

    // Serial.print(" measOmega = "); Serial.println(measuredOmega);
}

void Robot::brake(){
    digitalWrite(enableFL,LOW);
    digitalWrite(enableFR,LOW);
    digitalWrite(enableBL,LOW);
    digitalWrite(enableBR,LOW);

    digitalWrite(in1FL,LOW);
    digitalWrite(in2FL,LOW);
    digitalWrite(in1FR,LOW);
    digitalWrite(in2FR,LOW);
    digitalWrite(in1BL,LOW);
    digitalWrite(in2BL,LOW);
    digitalWrite(in1BR,LOW);
    digitalWrite(in2BR,LOW);

    digitalWrite(enableFL,HIGH);
    digitalWrite(enableFR,HIGH);
    digitalWrite(enableBL,HIGH);
    digitalWrite(enableBR,HIGH);
}

// DO NOT input any omegaOpp when calling
void Robot::omni4WD(long vfwd, long vhorz, long omega, long omegaOpposing) {

    // baking in the sqrt(2) factor: sqrt(2) * 256 * (45deg rotation) / 256
    long vFL = 361 * (vfwd + vhorz) / 256 - (centercorner * omega) / 64 + (centercorner * omegaOpposing) / 64;
    long vFR = 361 * (vfwd - vhorz) / 256 + (centercorner * omega) / 64 + (centercorner * omegaOpposing) / 64;
    long vBL = 361 * (vfwd - vhorz) / 256 - (centercorner * omega) / 64 - (centercorner * omegaOpposing) / 64;
    long vBR = 361 * (vfwd + vhorz) / 256 + (centercorner * omega) / 64 - (centercorner * omegaOpposing) / 64;

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

// in mm, mm, deg, sec
// rotation should be between 180 and -180
// i cant stop you if you put it outside itll just rotate multiple times
// either put 2 distances OR a rotation. this cannot handle both correctly.
void Robot::move3DOF_nofdbk(long ydist, long xdist, long rotation, long speed, long omega){

    long vx, vy;
    long dist = (int)(sqrt(sq(xdist)+sq(ydist)));
    vx = APPLYSIGN(speed,xdist) * xdist / dist;
    vy = APPLYSIGN(speed,ydist) * ydist / dist;
    omega = APPLYSIGN(omega,rotation);

    // internally in mm*20, mm*20, and 16384ths of rotation
    dist *= 20;
    xdist *= 20;
    ydist *= 20;
    rotation *= 16384;
    rotation /= 360;

    long measx = 0, measy = 0, measrot = 0;

    // Serial.print("dist = "); Serial.print(dist);
    // Serial.print(" measx = "); Serial.print(measx);
    // Serial.print(" measy = "); Serial.println(measy);
 
    // open loop
    omni4WD(vy,vx,omega);

    unsigned long newUpdate, lastUpdate = micros();

    while( ((sq(dist) - sq(measx) - sq(measy)) > 10000) || ((ABS(rotation - measrot)) > 256) ){
        newUpdate = micros();
        if(newUpdate - lastUpdate > timestep) {
            lastUpdate += timestep;

            measureSpeed();
            measy += measuredvFwd;
            measx += measuredvHorz;
            measrot += (measuredOmega/20);
            Serial.print("measured rotation = "); Serial.println(measrot);
            // Serial.print(measy); Serial.print(" = distance forward & distance horizontal = "); Serial.println(measx);
        }
    }
    Serial.print(measy); Serial.print(" = distance forward & distance horizontal = "); Serial.println(measx);
    Serial.println("done :)");
    brake();
}

// in mm, mm, deg, sec
// rotation should be between 180 and -180
// i cant stop you if you put it outside itll just rotate multiple times
void Robot::move3DOF(long ydist, long xdist, long rotation, int (Robot::*endcon)(void), long speed){

    long vx_des, vy_des;
    long dist = (int)(sqrt(sq(xdist)+sq(ydist)));
    vx_des = APPLYSIGN(speed,xdist) * xdist / dist;
    vy_des = APPLYSIGN(speed,ydist) * ydist / dist;
    long omega_des = defaultOmega*(APPLYSIGN(128,rotation));;

    // integrated units internally in mm*20, and 16384ths of rotation
    dist *= 20;
    xdist *= 20;
    ydist *= 20;
    rotation *= 16384;
    rotation /= 360;

    // Serial.print("dist = "); Serial.println(dist);

    long measx = 0, measy = 0, measrot = 0;

    long ei_y, ei_x, ei_omega;
    long ep_y, ep_x, ep_omega, ep_omegaOpp;
    long ed_y, ed_x, ed_omega, ed_omegaOpp;
    long vy = vy_des, vx = vx_des, omega = defaultOmega, omegaOpp = 0;

    omni4WD(vy,vx,omega,omegaOpp);
    measureSpeed();

    unsigned long newUpdate, lastUpdate = micros();

    // Serial.print("condition = "); Serial.println(sq(dist) - sq(measx) - sq(measy));
    while( ((sq(dist) - sq(measx) - sq(measy)) > 10000) || ((ABS(rotation - measrot)) > 128) ){
        newUpdate = micros();

        if((this->*endcon)() != 0){
            omni4WD(0,0,0);
            return;
        }

        if(newUpdate - lastUpdate > timestep) {
        // Serial.print("condition = "); Serial.println((ABS(rotation - measrot)) > 256);            
            lastUpdate += timestep;

            if(ydist - measy < 100){
                vy_des = 0;
            }
            if(xdist - measx < 100){
                vx_des = 0;
            }
            if(rotation - measrot < 128){
                omega_des = 0;
            }

            ed_y = ep_y;
            ed_x = ep_x;
            ed_omega = measuredOmega;
            ed_omegaOpp = measuredOppW;

            measureSpeed();

            measy += measuredvFwd;
            measx += measuredvHorz;
            measrot += (measuredOmega/20);
            // Serial.print("measured ydist = "); Serial.println(measy);
            // Serial.print("measured xdist = "); Serial.println(measx);
            // Serial.print("measured rotation = "); Serial.println(measrot);

            // Control

            // pos control
            ei_y = UPPERBOUND(ydist - measy,20);
            ei_x = UPPERBOUND(xdist - measx,40);
            ei_omega = UPPERBOUND(rotation - measrot,128);

            // vel control

            // noise rejection
            ep_y  = (vy_des - LOWERBOUND(measuredvFwd,20));
            ep_x = (vx_des - LOWERBOUND(measuredvHorz,20));

            ep_omega = (omega_des-measuredOmega);
            ep_omegaOpp = (0-measuredOppW);

            // vel control
            ed_y = ed_y - ep_y;
            ed_x = ed_x - ep_x;
            ed_omega = ed_omega - ep_omega;
            ed_omegaOpp = ed_omegaOpp - ep_omegaOpp;
            //*/

            Serial.print("vfwd ep = "); Serial.println(ep_y);

            vy += (ep_y*kp + ed_y*kd)/32;
            vx += (ep_x*kp + ed_x*kd)/32;
            omega += (ep_omega*kp_omega + ed_omega*kd_omega) / 2048;
            omegaOpp += (ep_omegaOpp*kp_omegaOpp + ed_omegaOpp*kd_omegaOpp) / 2048;

            Serial.print("vfwd set = "); Serial.println(vy);

            omni4WD(vy + (ei_y*ki)/32, vx + (ei_x*ki)/32, omega + (ei_omega*ki_omega/1024), omegaOpp);
            // Serial.println("timestamp check");
        }
    }
    Serial.print("distance forward = "); Serial.print(measy);
    Serial.print(" & distance horizontal = "); Serial.println(measx);
    Serial.print("total rotation = "); Serial.println(measrot);
    Serial.println("done :)");
    brake();
    // omni4WD(0,0,0); // coast
}

// in mm, mm, deg, sec
// rotation should be between 180 and -180
// i cant stop you if you put it outside itll just rotate multiple times
void Robot::move3DOF_heading(long ydist, long xdist, long rotation, int (Robot::*endcon)(void), long speed){

    long vx_des, vy_des;
    long dist = (long)(sqrt(sq(xdist)+sq(ydist)));

    // These need to be in the same "units" as the measurements
    vx_des = 2*speed * xdist / dist;
    vy_des = 2*speed * ydist / dist;
    long omega_des = defaultOmega*(APPLYSIGN(128,rotation));

    // Serial.print("vy_des = "); Serial.println(vy_des);

    // integrated units internally in mm*20, and 16384ths of rotation
    dist *= 20;
    xdist *= 20;
    ydist *= 20;
    rotation *= 16384;
    rotation /= 360;

    long measx = 0, measy = 0, measrot = 0;
    long curCos, curSin;

    // in order: error for vfwd, vhorz, omega, and omegaOpp
    long ei_y, ei_x, ei_omega, iHorz, iFwd;
    long ep_y, ep_x, ep_omega, ep_omegaOpp;
    long ed_y, ed_x, ed_omega, ed_omegaOpp;
    long vy = vy_des, vx = vx_des, vHorz, vFwd, omega = defaultOmega, omegaOpp = 0;
 
    omni4WD(vy,vx,omega,omegaOpp);
    measureSpeed();

    unsigned long newUpdate, lastUpdate = micros();

    // Serial.print("condition = "); Serial.println(sq(dist) - sq(measx) - sq(measy));
    while( ((sq(dist) - sq(measx) - sq(measy)) > 10000) || ((ABS(rotation - measrot)) > 128) ){
        newUpdate = micros();

        if((this->*endcon)() != 0){
            omni4WD(0,0,0);
            return;
        }

        if(newUpdate - lastUpdate > timestep) {
        // Serial.print("condition = "); Serial.println((ABS(rotation - measrot)) > 256);            
            lastUpdate += timestep;

            if(ABS(measy) > ABS(ydist) + 100){
                vy_des = 0;
                ep_y = 0;
                vy = 0;
            }
            if(ABS(measx) > ABS(xdist) + 100){
                vx_des = 0;
                ep_x = 0;
                vx = 0;
            }
            if(ABS(measrot) > ABS(rotation) + 128){
                omega_des = 0;
                ep_omega = 0;
                omega = 0;
            }

            ed_y = ep_y;
            ed_x = ep_x;
            ed_omega = measuredOmega;
            ed_omegaOpp = measuredOppW;

            measureSpeed();

            measrot += (measuredOmega/20);
            curCos = myCos(measrot);
            curSin = mySin(measrot);

            // need to account for rotation
            long measuredvx = curCos*measuredvHorz/256 - curSin*measuredvFwd/256;
            long measuredvy = curSin*measuredvHorz/256 + curCos*measuredvFwd/256;

            measy += measuredvy;
            measx += measuredvx;

            // Control

            // pos control
            ei_y = (ydist > 0) ? min(ydist - measy, 40) : max(ydist - measy, -40);
            ei_x = (xdist > 0) ? min(xdist - measx, 40) : max(xdist - measx, -40);
            ei_omega = (rotation > 0) ? min(rotation - measrot, 128) : max(rotation - measrot, -128);

            // need to account for rotation
            // vel control

            // noise rejection
            ep_y  = (vy_des - LOWERBOUND(measuredvy,20));
            ep_x = (vx_des - LOWERBOUND(measuredvx,20));
            
            ep_omega = (omega_des - measuredOmega);
            ep_omegaOpp = (0 - measuredOppW);

            // vel control
            ed_y = ed_y - ep_y;
            ed_x = ed_x - ep_x;
            ed_omega = ed_omega - ep_omega;
            ed_omegaOpp = ed_omegaOpp - ep_omegaOpp;
            //*/

            vy += (ep_y*kp + ed_y*kd)/32;
            vx += (ep_x*kp + ed_x*kd)/32;
            omega += (ep_omega*kp_omega + ed_omega*kd_omega) / 2048;
            omegaOpp += (ep_omegaOpp*kp_omegaOpp + ed_omegaOpp*kd_omegaOpp) / 2048;

            Serial.print("measured rot = "); Serial.println(measrot);
            // Serial.print("ei omega = "); Serial.println(ei_omega);

            // need to account for rotation
            // transform x,y back into fwd, horz
            vHorz = curCos*vx/256 + curSin*vy/256;
            vFwd = -curSin*vx/256 + curCos*vy/256;

            // div by 256, div by 32
            iHorz = curCos*(ei_x*ki)/8192 + curSin*(ei_y*ki)/8192;
            iFwd = -curSin*(ei_x*ki)/8192 + curCos*(ei_y*ki)/8192;

            // Serial.print("vfwd pos error = "); Serial.println(iFwd);
            omni4WD(vFwd + iFwd,vHorz + iHorz,omega + (ei_omega*ki_omega/128),omegaOpp);
            // Serial.println("timestamp check");
        }
    }
    /*
    Serial.print("distance forward = "); Serial.print(measy);
    Serial.print(" & distance horizontal = "); Serial.println(measx);
    Serial.print("total rotation = "); Serial.println(measrot);
    Serial.println("done :)");
    //*/
    brake();
}

void Robot::servoPosition(int angle){
    armServo.write(angle);
}

void Robot::rightTurn(short vfwd) {
    omni4WD(vfwd, 0, -64);

    return;
}

void Robot::leftTurn(short vfwd) {
    omni4WD(vfwd, 0, 64);

    return;
}


int Robot::readBlackSenses() {
    // Serial.print("black senses: "); Serial.println(blackSenses);
    return blackSenses;
}

int Robot::scanReadSenses() {
    // combines measureLine with readBlackSenses
    measureLine();
    return blackSenses;
}

void Robot::senseColor() {
    // calculates and stores colorTemp and lux values
    tcs.getRawData(&r, &g, &b, &c);
    colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
    lux = tcs.calculateLux(r, g, b);

    return;
}

long *Robot::readColors() {
    // returns colors as an rgb array
    long colors[3] = {r, g, b};
    return colors;
}

int Robot::colorDetect() {
    senseColor();
    if (lux > 250)
        return 0;
    return 1;
}