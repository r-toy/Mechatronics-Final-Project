#include <Arduino.h>
#include "robot.h"
#include "context.h"
#include "state.h"
#include "StateButtonRead.h"

void StateButtonRead::enter(){
    Serial.println("entering state ButtonRead");
    ctx_->ourRobot->omni4WD(vfwdSetpoint, 0, 0);
    delay(1750);
    ctx_->ourRobot->omni4WD(-vfwdSetpoint, 0, 0);
    delay(100);
    ctx_->ourRobot->omni4WD(0, 0, -64);
    delay(750);
    ctx_->ourRobot->omni4WD(vfwdSetpoint, 0, 0);
    delay(250);
    ctx_->ourRobot->senseColor();

}

void StateButtonRead::update(){
    ctx_->transitionTo(new StateHorizontalLine);
}

void StateButtonRead::exit() {
    Serial.println("exiting state ButtonRead");
    ctx_->ourRobot->leftTurn(vfwdSetpoint);
    lastUpdate = micros();
    while (1) {
        newUpdate = micros();
        if (newUpdate - lastUpdate > ctx_->ourRobot->timestep) {
            ctx_->ourRobot->measureLine();
            if (ctx_->ourRobot->readBlackSenses() == 0)
                break;
        }
    }

    return;
}
