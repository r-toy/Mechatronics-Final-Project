#include "StateDance.h"
#include "context.h"
#include "robot.h"

void StateDance::enter(){
    ctx_->ourRobot->move3DOF_heading(0,100,-90);
    lastUpdate = micros();
}

void StateDance::update(){
    newUpdate = micros();
    if(newUpdate - lastUpdate > ctx_->ourRobot->timestep){
        lastUpdate += ctx_->ourRobot->timestep;

        // dance code
        ctx_->ourRobot->measureSpeed();
        measrot += ctx_->ourRobot->measuredOmega/20;

        long vYcurrent = vYset*ctx_->ourRobot->myCos(phase)/256;
        long vFwd = ctx_->ourRobot->myCos(measrot)*vYcurrent/256;
        long vHorz = ctx_->ourRobot->mySin(measrot)*vYcurrent/256;

        phase += 16;
        
        ctx_->ourRobot->omni4WD(vFwd,vHorz,omega);

        int servoPos = 180*(measrot % 4096) / 4096 + 2048;
        ctx_->ourRobot->servoPosition(servoPos);
    }
}