#include <Arduino.h>
#include "robot.h"
#include "context.h"
#include "state.h"
#include "StateMaze1.h"

void StateMaze1::update(){
    newUpdate = micros();

    if( (newUpdate - lastUpdate) > ctx_->ourRobot->timestep){
        lastUpdate += ctx_->ourRobot->timestep;

        // control loop update
        ctx_->ourRobot->measureSpeed();
        // ep = ctx_->ourRobot->measBalance + ctx_->ourRobot->measureLine();
        ed = ep;
        ep = ctx_->ourRobot->measureLine();
        ed = ep - ed;
        currentBalance = ep*kp + ed*kd;
        // Serial.println(currentBalance);
        // currentSpeed = currentSpeed + (speedSetpoint - ctx_->ourRobot->measSpeed)*kp;
        if (ctx_->ourRobot->readBlacks() == 0) {
            ctx_->transitionTo(new )
        }
        ctx_->ourRobot->diffDrive(speedSetpoint, currentBalance);
    }

    //if (ctx_->ourRobot->readPushbutton) ctx_->transitionTo(new StatePivotRight)
}

void StateMaze1::exit() {
    ctx_->ourRobot->diffDrive(speedSetpoint, 0);
    delay(1000);
    ctx_->ourRobot->leftTurn(speedSetpoint);
    ctx_->ourRobot->diffDrive(speedSetpoint, 0);

    return;
}
