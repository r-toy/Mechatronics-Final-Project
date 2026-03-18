#include <Arduino.h>
#include "robot.h"
#include "context.h"
#include "mymacros.h"
#include "state.h"
#include "StateButtonRead.h"
#include "StateBarRed.h"
#include "StateBlue.h"
#include "StateGreen.h"
#include "StateRedReverse.h"
#include "StateYellow.h"

void StateBarRed::update(){
    if (!ctx_->ourRobot->colorDetect())
        ctx_->transitionTo(new StateRedReverse);
}