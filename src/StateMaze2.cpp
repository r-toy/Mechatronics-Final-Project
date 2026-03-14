#include <Arduino.h>
#include "robot.h"
#include "context.h"
#include "state.h"
#include "StateMaze2.h"
#include "StateMaze3.h"

void StateMaze2::update(){
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
        // Serial.println(currentBalance);
        // currentSpeed = currentSpeed + (speedSetpoint - ctx_->ourRobot->measSpeed)*kp;
        if (ctx_->ourRobot->readBlacks() == 0) {
            ctx_->transitionTo(new StateMaze3);
        }
        ctx_->ourRobot->omni4WD(vfwdSetpoint, vhorzSetpoint, currentOmega);
        ctx_->ourRobot->resetBlacks();
    }

    //if (ctx_->ourRobot->readPushbutton) ctx_->transitionTo(new StatePivotRight)
}

void StateMaze2::exit() {
    ctx_->ourRobot->omni4WD(vfwdSetpoint, 0, 0);
    delay(800);
    ctx_->ourRobot->rightTurn(vfwdSetpoint);
    ctx_->ourRobot->omni4WD(vfwdSetpoint, 0, 0);
    delay(1500);

    return;
}