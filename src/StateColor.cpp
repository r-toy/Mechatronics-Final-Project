#include <Arduino.h>
#include "robot.h"
#include "context.h"
#include "state.h"
#include "StateTEST.h"
#include "StateColor.h"
#include "StateBar.h"

void StateColor::enter(){
    Serial.println("entering state Color");
}

void StateColor::update(){
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
        // LINE COUNTING
        if (ctx_->ourRobot->readBlackSenses() > 4) {
            if ((--color) == 0)
                ctx_->transitionTo(new StateBar);
            while (ctx_->ourRobot->scanReadSenses() > 4){

            }
        }
        ctx_->ourRobot->omni4WD(vfwdSetpoint, currentVhorz, currentOmega);
    }

    //if (ctx_->ourRobot->readPushbutton) ctx_->transitionTo(new StatePivotRight)
}

void StateColor::exit() {
    Serial.println("exiting state color");
    ctx_->ourRobot->move3DOF_heading(40, 0, 0);

    return;
}