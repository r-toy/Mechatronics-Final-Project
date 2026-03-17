#include "robot.h"
#include "context.h"
#include "state.h"
#include "StateDrift.h"

void StateDrift::enter() {
    ctx_->ourRobot->omni4WD(0,25,200);
}

void StateDrift::update(){

}   

void StateDrift::exit(){

}
