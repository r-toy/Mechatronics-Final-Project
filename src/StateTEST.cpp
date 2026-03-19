#include <Arduino.h>
#include "robot.h"
#include "StateTEST.h"
#include "context.h"
#include "mymacros.h"

void StateTEST::enter() {
    Serial.println("entering state test");
    // lastUpdate = micros();
    // startTime = lastUpdate;
    // ctx_->ourRobot->brake();
    // ctx_->ourRobot->move3DOF_nofdbk(0,0,90);    
    // ctx_->ourRobot->move3DOF_heading(-100,0,-90);
    // delay(125);
    // ctx_->ourRobot->move3DOF_heading(0,0,-90);
    // ctx_->ourRobot->move3DOF(0,0,180);
}

void StateTEST::update() {
    // ctx_->ourRobot->brake();
    unsigned short colors[4];
    unsigned short red = 0, green = 0, blue = 0, clear = 0;
    ctx_->ourRobot->senseColor();
    ctx_->ourRobot->readColors(colors);
    for (int i = 0; i < 64; i++){
        red += colors[0];
        green += colors[1];
        blue += colors[2];
        clear += colors[3];
    }
    Serial.print("red: "); Serial.print(red/64);
    Serial.print(" green: "); Serial.print(green/64);
    Serial.print(" blue: "); Serial.print(blue/64);
    Serial.print(" clear: "); Serial.println(clear/64);
    delay(1000);
    // ctx_->ourRobot->servoPosition(90);
    // delay(500);
    // ctx_->ourRobot->servoPosition(0);
    // delay(500);
    // ctx_->ourRobot->servoPosition(180);
    // delay(500);



    // Calibration for measureSpeed()
    /*
    newUpdate = micros();
    currentTime = newUpdate;

    if( currentTime - startTime > moveTime) {
        Serial.print("Average omega = "); Serial.println(angle/numMeas);
        Serial.print("rotation traveled = "); Serial.println(angle / 20);
        ctx_->ourRobot->brake();
        while(1);
    }

    else if( (newUpdate - lastUpdate) > ctx_->ourRobot->timestep){
        lastUpdate += ctx_->ourRobot->timestep;

        ctx_->ourRobot->measureSpeed();
        numMeas++;
        distance += ctx_->ourRobot->measuredvFwd;
        angle += ctx_->ourRobot->measuredOmega;
        Serial.print("Measured omega = "); Serial.println(ctx_->ourRobot->measuredOmega);

        ctx_->ourRobot->omni4WD(100,0,0);

    }
    //*/
}

void StateTEST::exit() {

}
