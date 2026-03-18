#include <Arduino.h>
#include "robot.h"
#include "context.h"
#include "StateTEST.h"
#include "state.h"
#include "StateMaze3.h"
#include "StateButtonRead.h"
#include "StateDrift.h"

void StateMaze3::enter(){
    Serial.println("entering state 3");
    delay(500);
}

void StateMaze3::update(){
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
        maxSenses = max(maxSenses, ctx_->ourRobot->readBlackSenses());
        int isthebuttonpushed = ctx_->ourRobot->pushbuttonRead();
        if ((maxSenses > 4) || isthebuttonpushed) {
            Serial.print("max senses = "); Serial.println(maxSenses);
            ctx_->transitionTo(new StateTEST);
        }
        ctx_->ourRobot->omni4WD(maze3vFwd, currentVhorz, currentOmega);
    }

    //if (ctx_->ourRobot->readPushbutton) ctx_->transitionTo(new StatePivotRight)
}

void StateMaze3::exit() {
    Serial.println("exiting state 3");
    ctx_->ourRobot->brake();
}