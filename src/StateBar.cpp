#include <Arduino.h>
#include "robot.h"
#include "context.h"
#include "mymacros.h"
#include "state.h"
#include "StateBar.h"
#include "StateReverse.h"

void StateBar::enter(){
    Serial.println("entering state Bar");
    ctx_->ourRobot->servoPosition(180);
    ctx_->ourRobot->move3DOF_heading(10, -340, 0);
    ctx_->ourRobot->servoPosition(90);
    delay(100);
    int mill = millis();
    ctx_->ourRobot->omni4WD(0, 255, 0);
    Serial.println("HELP ME PLEASE");
    while(millis() - mill < 250){

    }
    ctx_->ourRobot->brake();
    Serial.println("HELP ME");
    delay(125);



}

void StateBar::update(){
    // if (!ctx_->ourRobot->colorDetect())
    delay(1000);
    unsigned short colors[4];
    unsigned short red = 0, green = 0, blue = 0, clear = 0;
    ctx_->ourRobot->senseColor();
    ctx_->ourRobot->readColors(colors);
    for (int i = 0; i < 64; i++){
        red += colors[0];
        green += colors[1];
        blue += colors[2];
        clear += colors[3];
    }
    Serial.print("red: "); Serial.print(red/64);
    Serial.print(" green: "); Serial.print(green/64);
    Serial.print(" blue: "); Serial.print(blue/64);
    Serial.print(" clear: "); Serial.println(clear/64);
    delay(1000);
    if (ctx_->ourRobot->pushbuttonRead() == 1)
        ctx_->transitionTo(new StateTEST);
}

void StateBar::exit() {
    Serial.println("exiting state Bar");
    int mill = millis();
    ctx_->ourRobot->omni4WD(0, 0, 0);
    while(millis() - mill < 500){

    }
    ctx_->ourRobot->brake();

    ctx_->ourRobot->servoPosition(90);
    delay(1000);
    // ctx_->ourRobot->move3DOF_heading(0, 100, 180);
    // ctx_->ourRobot->move3DOF_heading(0, -500, 0, &Robot::scanReadSenses);

    // ctx_->ourRobot->servoPosition(180);

    return;
}
