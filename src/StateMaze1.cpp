#include <Arduino.h>
#include "robot.h"
#include "context.h"
#include "state.h"
#include "StateTEST.h"
#include "StateMaze1.h"
#include "StateMaze2.h"

void StateMaze1::enter(){
    Serial.println("entering state 1");
}

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
        currentOmega = ep*kp + ed*kd;
        currentVhorz = ep/8;
        // Serial.println(currentBalance);
        // currentSpeed = currentSpeed + (speedSetpoint - ctx_->ourRobot->measSpeed)*kp;
        if (ctx_->ourRobot->readBlackSenses() == 0) {
            ctx_->transitionTo(new StateMaze2);
        }
        ctx_->ourRobot->omni4WD(vfwdSetpoint, currentVhorz, currentOmega);
    }

    //if (ctx_->ourRobot->readPushbutton) ctx_->transitionTo(new StatePivotRight)
}

void StateMaze1::exit() {
    Serial.println("exiting state 1");
    ctx_->ourRobot->move3DOF_heading(275, 0, 90);
    delay(125);
    ctx_->ourRobot->move3DOF_heading(500, 0, 90, &Robot::scanReadSenses);
    // ctx_->ourRobot->omni4WD(0,0,70);
    // lastUpdate = micros();
    // while (1) {
    //     newUpdate = micros();
    //     if (newUpdate - lastUpdate > ctx_->ourRobot->timestep) {
    //         lastUpdate += ctx_->ourRobot->timestep;
    //         ctx_->ourRobot->measureLine();
    //         if (ctx_->ourRobot->readBlackSenses() != 0)
    //             break;
    //     }
    // }
    // while(ctx_->ourRobot->readBlackSenses() == 0){
    //     ctx_->ourRobot->measureLine();
    // }

    return;
}
