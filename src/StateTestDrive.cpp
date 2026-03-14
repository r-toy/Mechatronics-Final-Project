#include <Arduino.h>
#include "robot.h"
#include "context.h"
#include "state.h"
#include "StateTestDrive.h"

void StateTestDrive::enter() {
    
}

void StateTestDrive::update(){
    // left forward
    ctx_->ourRobot->diffDrive(120, 64);
    delay(1000);
    ctx_->ourRobot->diffDrive(0, 0);
    delay(1000);
    // right backward
    ctx_->ourRobot->diffDrive(-120, 64);
    delay(1000);
    ctx_->ourRobot->diffDrive(0, 0);
    delay(1000);
    // left backwards
    ctx_->ourRobot->diffDrive(-120, -64);
    delay(1000);
    ctx_->ourRobot->diffDrive(0, 0);
    delay(1000);
    // right forward
    ctx_->ourRobot->diffDrive(120, -64);
    delay(1000);
    ctx_->ourRobot->diffDrive(0, 0);
    delay(1000);
}   

void StateTestDrive::exit(){

}