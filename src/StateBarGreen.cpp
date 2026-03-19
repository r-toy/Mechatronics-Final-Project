#include <Arduino.h>
#include "robot.h"
#include "context.h"
#include "mymacros.h"
#include "state.h"
#include "StateButtonRead.h"
#include "StateBarGreen.h"
#include "StateBlue.h"
#include "StateGreenReverse.h"
#include "StateRed.h"
#include "StateYellow.h"
#include "StateTEST.h"

void StateBarGreen::update(){
    if (ctx_->ourRobot->colorDetect())
        ctx_->transitionTo(new StateGreenReverse);
    // ctx_->transitionTo(new StateTEST);
}