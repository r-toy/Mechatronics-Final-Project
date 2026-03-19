#include <Arduino.h>
#include "robot.h"
#include "context.h"
#include "state.h"
#include "StateLineFollow.h"

void StateLineFollow::enter() {
    lastUpdate = micros();
}

void StateLineFollow::update(){
    newUpdate = micros();

    if( (newUpdate - lastUpdate) > ctx_->ourRobot->timestep){
        lastUpdate += ctx_->ourRobot->timestep;

        // control loop update
        // ep = ctx_->ourRobot->measBalance + ctx_->ourRobot->measureLine();
        ed = ep;
        ep = ctx_->ourRobot->measureLine();
        ed = ep - ed;
        currentOmega = ep*kp + ed*kd;
        currentVhorz = ep/8;
        // currentSpeed = currentSpeed + (speedSetpoint - ctx_->ourRobot->measSpeed)*kp;
        ctx_->ourRobot->omni4WD(vfwdSetpoint, currentVhorz, currentOmega);
    }

    //if (ctx_->ourRobot->readPushbutton) ctx_->transitionTo(new StatePivotRight)
}   

void StateLineFollow::exit(){

}
