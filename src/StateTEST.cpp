#include <Arduino.h>
#include "robot.h"
#include "StateTEST.h"
#include "context.h"

void StateTEST::enter() {
    Serial.println("entering state test");
}

void StateTEST::update() {
        lastUpdate += ctx_->ourRobot->timestep;

        // control loop update
        ctx_->ourRobot->measureSpeed();
        Serial.print("measVfwd = "); Serial.println(ctx_->ourRobot->measuredvFwd);
        distance += (long)ctx_->ourRobot->measuredvFwd*(ctx_->ourRobot->timestep/1000)/1000;
        // ep = ctx_->ourRobot->measBalance + ctx_->ourRobot->measureLine();
        ed = ep;
        ep = ctx_->ourRobot->measureLine();
        ed = ep - ed;
        currentOmega = ep*kp + ed*kd;
        currentVhorz = ep/8;
        // Serial.println(currentBalance);
        // currentSpeed = currentSpeed + (speedSetpoint - ctx_->ourRobot->measSpeed)*kp;
        if (ctx_->ourRobot->readBlackSenses() == 0) {
            ctx_->ourRobot->brake();
            Serial.print("distance = "); Serial.println(distance);
            while(1);
        }
        ctx_->ourRobot->omni4WD(vfwdSetpoint, currentVhorz, currentOmega);

    //if (ctx_->ourRobot->readPushbutton) ctx_->transitionTo(new StatePivotRight)    
}

void StateTEST::exit() {

}
