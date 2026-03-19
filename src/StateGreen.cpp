#include <Arduino.h>
#include "robot.h"
#include "context.h"
#include "state.h"
#include "StateTEST.h"
#include "StateGreen.h"
#include "StateBarGreen.h"

void StateGreen::update(){
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
        // LINE COUNTING
        Serial.print("black: "); Serial.println(ctx_->ourRobot->readBlackSenses());
        if (ctx_->ourRobot->readBlackSenses() >= 4) {
            Serial.print("zeros; "); Serial.println(color);
            if (!(--color))
                ctx_->transitionTo(new StateBarGreen);
            while (ctx_->ourRobot->scanReadSenses() >= 4){
                delay(5);
            }
        }
        ctx_->ourRobot->omni4WD(vfwdSetpoint, currentVhorz, currentOmega);
    }

    //if (ctx_->ourRobot->readPushbutton) ctx_->transitionTo(new StatePivotRight)
}