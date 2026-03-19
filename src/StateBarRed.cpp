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
#include "StateTEST.h"

void StateBarRed::update(){
    if (ctx_->ourRobot->colorDetect())
        ctx_->transitionTo(new StateRedReverse);
    // delay(1000);
    // unsigned short colors[4];
    // unsigned short red = 0, green = 0, blue = 0, clear = 0;
    // ctx_->ourRobot->senseColor();
    // ctx_->ourRobot->readColors(colors);
    // for (int i = 0; i < 64; i++){
    //     red += colors[0];
    //     green += colors[1];
    //     blue += colors[2];
    //     clear += colors[3];
    // }
    // Serial.print("red: "); Serial.print(red/64);
    // Serial.print(" green: "); Serial.print(green/64);
    // Serial.print(" blue: "); Serial.print(blue/64);
    // Serial.print(" clear: "); Serial.println(clear/64);
    // delay(1000);
    // if (ctx_->ourRobot->pushbuttonRead() == 1)
    //     ctx_->transitionTo(new StateTEST);
    // ctx_->transitionTo(new StateTEST);
}