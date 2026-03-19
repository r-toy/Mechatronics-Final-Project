#include <Arduino.h>
#include "robot.h"
#include "context.h"
#include "mymacros.h"
#include "state.h"
#include "StateBar.h"
#include "StateReverse.h"

void StateBar::enter(){
    Serial.println("entering state Bar");
    // ctx_->ourRobot->servoPosition(180);
    // ctx_->ourRobot->move3DOF_heading(0, 400, 0);
    ctx_->ourRobot->servoPosition(90);
    delay(100);
    int mill = millis();
    ctx_->ourRobot->omni4WD(0, 255, 0);
    while(millis() - mill < 500){

    }
    ctx_->ourRobot->brake();
}

void StateBar::update(){
    // if (!ctx_->ourRobot->colorDetect())
    delay(1000);
    ctx_->transitionTo(new StateReverse);
}

void StateBar::exit() {
    Serial.println("exiting state Bar");
    int mill = millis();
    ctx_->ourRobot->omni4WD(0, -255, 0);
    while(millis() - mill < 500){

    }
    ctx_->ourRobot->brake();

    ctx_->ourRobot->servoPosition(135);
    delay(1000);
    ctx_->ourRobot->move3DOF_heading(0, 100, 180);
    ctx_->ourRobot->move3DOF_heading(0, -500, 0, &Robot::scanReadSenses);

    ctx_->ourRobot->servoPosition(180);

    return;
}
