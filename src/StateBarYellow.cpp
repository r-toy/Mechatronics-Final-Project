#include <Arduino.h>
#include "robot.h"
#include "context.h"
#include "mymacros.h"
#include "state.h"
#include "StateButtonRead.h"
#include "StateBarYellow.h"
#include "StateBlue.h"
#include "StateGreen.h"
#include "StateRed.h"
#include "StateYellowReverse.h"

void StateBarYellow::update(){
    if (!ctx_->ourRobot->colorDetect())
        ctx_->transitionTo(new StateYellowReverse);
}