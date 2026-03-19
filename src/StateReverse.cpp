#include <Arduino.h>
#include "robot.h"
#include "context.h"
#include "state.h"
#include "StateButtonRead.h"
#include "StateReverse.h"

void StateReverse::enter(){
    color = ctx_->color;
}

void StateReverse::update(){
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

        // LINE COUNTING
        if (ctx_->ourRobot->readBlackSenses() >= 4) {
            if ((--color) != 0)
                ctx_->transitionTo(new StateButtonRead);
            while (ctx_->ourRobot->scanReadSenses() >= 4){
                delay(5);
            }
        }
        ctx_->ourRobot->omni4WD(vfwdSetpoint, currentVhorz, currentOmega);
    }

    //if (ctx_->ourRobot->readPushbutton) ctx_->transitionTo(new StatePivotRight)
}

void StateReverse::exit() {
    ctx_->ourRobot->move3DOF_heading(20, 0, -90);
}