#include <Arduino.h>
#include "robot.h"
#include "StateTEST.h"
#include "context.h"

void StateTEST::enter() {
    ctx_->ourRobot->omni4WD(0,0,0);
}

void StateTEST::update() {
    ctx_->ourRobot->omni4WD(0,0,0);
    ctx_->ourRobot->servoPosition(0);
    delay(1000);
    ctx_->ourRobot->servoPosition(90);
    delay(1000);
    ctx_->ourRobot->servoPosition(180);
    delay(1000);
    ctx_->ourRobot->servoPosition(90);
    delay(1000);
    
}

void StateTEST::exit() {

}
