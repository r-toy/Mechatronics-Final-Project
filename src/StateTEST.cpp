#include <Arduino.h>
#include "robot.h"
#include "StateTEST.h"
#include "context.h"
#include "mymacros.h"

void StateTEST::enter() {
    Serial.println("entering state test");
    lastUpdate = micros();
    startTime = lastUpdate;
    // ctx_->ourRobot->move3DOF_nofdbk(0,0,90);    
    ctx_->ourRobot->move3DOF_heading(100,0,0);   
    delay(125);
    // ctx_->ourRobot->move3DOF(0,0,180);
}

void StateTEST::update() {
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
