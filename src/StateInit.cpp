#include "StateInit.h"
#include "StateLineFollow.h"
#include "StateTEST.h"
#include "StateMaze1.h"
#include "StateMaze3.h"
#include "StateButtonRead.h"
#include "context.h"
#include "robot.h"
#include "StateBar.h"

void StateInit::enter() {
    // calibrate line sensor
    while (ctx_->ourRobot->pushbuttonRead() == 0){
        // wait for button
    }
    // ctx_->transitionTo(new StateButtonRead);
    // ctx_->transitionTo(new StateButtonRead);
    //*
    ctx_->ourRobot->calibrateLineSensor();
    delay(125);
    ctx_->ourRobot->move3DOF_heading(-100,0,-90);
    ctx_->ourRobot->brake();
    delay(125);
    ctx_->ourRobot->calibrateLineSensor();
    delay(125);
    ctx_->ourRobot->move3DOF_heading(0,0,-90,&Robot::scanReadSenses);
    //*
    // while (ctx_->ourRobot->pushbuttonRead() == 0){
    //     // wait for button
    // }
    ctx_->transitionTo(new StateMaze1);
}

void StateInit::update() {
    if(ctx_->ourRobot->pushbuttonRead() == 1)
        ctx_ -> transitionTo(new StateMaze1);
}

void StateInit::exit() {

}
