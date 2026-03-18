#include <Arduino.h>
#include "robot.h"
#include "context.h"
#include "mymacros.h"
#include "state.h"
#include "StateButtonRead.h"
#include "StateBarBlue.h"
#include "StateBlueReverse.h"
#include "StateGreen.h"
#include "StateRed.h"
#include "StateYellow.h"

void StateBarBlue::update(){
    if (!ctx_->ourRobot->colorDetect())
        ctx_->transitionTo(new StateBlueReverse);
}