#include "StateInit.h"
#include "StateLineFollow.h"
#include "StateTEST.h"
#include "StateMaze1.h"
#include "context.h"
#include "robot.h"

void StateInit::enter() {
    // calibrate line sensor
    for (int i = 0; i<200; i++){
        // calibration call
    }

}

void StateInit::update() {
    if(ctx_->ourRobot->pushbuttonRead() == 1)
        ctx_ -> transitionTo(new StateMaze1);
}

void StateInit::exit() {

}
