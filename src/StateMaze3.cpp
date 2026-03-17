#include <Arduino.h>
#include "robot.h"
#include "context.h"
#include "StateTEST.h"
#include "state.h"
#include "StateMaze3.h"
#include "StateButtonRead.h"

void StateMaze3::enter(){
    Serial.println("entering state 3");
}

void StateMaze3::update(){
    newUpdate = micros();

    if( (newUpdate - lastUpdate) > ctx_->ourRobot->timestep){
        lastUpdate += ctx_->ourRobot->timestep;

        // control loop update
        ctx_->ourRobot->measureSpeed();
        // ep = ctx_->ourRobot->measBalance + ctx_->ourRobot->measureLine();
        ed = ep;
        ep = ctx_->ourRobot->measureLine();
        ed = ep - ed;
        currentOmega = ep*kp + ed*kd;
        currentVhorz = ep/8;
        // Serial.println(currentBalance);
        // currentSpeed = currentSpeed + (speedSetpoint - ctx_->ourRobot->measSpeed)*kp;
        if (ctx_->ourRobot->readBlackSenses() > 4) {
            ctx_->transitionTo(new StateButtonRead);
        }
        ctx_->ourRobot->omni4WD(vfwdSetpoint, currentVhorz, currentOmega);
    }

    //if (ctx_->ourRobot->readPushbutton) ctx_->transitionTo(new StatePivotRight)
}

void StateMaze3::exit() {
    Serial.println("exiting state 3");
    ctx_->ourRobot->brake();
}