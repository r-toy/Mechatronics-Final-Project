#include <Arduino.h>
#include "robot.h"
#include "context.h"
#include "state.h"
#include "StateMaze2.h"
#include "StateMaze3.h"

void StateMaze2::enter(){
    Serial.println("entering state 2");
}

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
        currentVhorz = ep/2;
        // Serial.println(currentBalance);
        // currentSpeed = currentSpeed + (speedSetpoint - ctx_->ourRobot->measSpeed)*kp;
        if (ctx_->ourRobot->readBlackSenses() == 0) {
            ctx_->transitionTo(new StateMaze3);
        }
        ctx_->ourRobot->omni4WD(vfwdSetpoint, currentVhorz, currentOmega);
    }

    //if (ctx_->ourRobot->readPushbutton) ctx_->transitionTo(new StatePivotRight)
}

void StateMaze2::exit() {
    // short omega = -80, increment = 1;
    // long update2 = 0, lastUpdate2 = micros();
    Serial.println("exiting state 2");
    ctx_->ourRobot->move3DOF_heading(225, 0, 0);
    ctx_->ourRobot->move3DOF_heading(-100, 500, -180, &Robot::scanReadSenses);
    // ctx_->ourRobot->omni4WD(vfwdSetpoint,0,-74);
    // lastUpdate = micros();
    // while (1) {
    //     newUpdate = micros();
    //     if (newUpdate - lastUpdate > ctx_->ourRobot->timestep) {
    //         lastUpdate += ctx_->ourRobot->timestep;
    //         update2 = micros();
    //         if (update2 - lastUpdate2 > timer) {
    //             if (omega == -74)
    //                 increment = -1;
    //             lastUpdate2 = micros();
    //             omega += increment;
    //             ctx_->ourRobot->omni4WD(vfwdSetpoint,0,omega);
    //         }
    //         ctx_->ourRobot->measureLine();
    //         if (ctx_->ourRobot->readBlackSenses() != 0)
    //             break;
    //     }
    // }


    return;
}