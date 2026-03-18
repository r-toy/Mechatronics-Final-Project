#include <Arduino.h>
#include "robot.h"
#include "context.h"
#include "mymacros.h"
#include "state.h"
#include "StateButtonRead.h"
#include "StateRed.h"
#include "StateGreen.h"
#include "StateBlue.h"
#include "StateYellow.h"

float StateButtonRead::vDistance(const int *one, const int *two){
  int dsqr = 0;
  for (int i = 0; i < 3; i++) {
    dsqr += (one[i] - two[i]) * (one[i] - two[i]);
  }
  return sqrt((double)(dsqr));
}


void StateButtonRead::enter(){
    Serial.println("entering state ButtonRead");
    ctx_->ourRobot->move3DOF_heading(300, 0, 0, &Robot::defaultEndcon, 255);
    delay(125);
    Serial.println("button pressed");

    lastUpdate = micros();
    int starttime = micros();
    // realign to line
    while(newUpdate - starttime < 1000000){
        newUpdate = micros();

        if( (newUpdate - lastUpdate) > ctx_->ourRobot->timestep){
            lastUpdate += ctx_->ourRobot->timestep;

            // control loop update
            ctx_->ourRobot->measureSpeed();
            // ep = ctx_->ourRobot->measBalance + ctx_->ourRobot->measureLine();
            ed = ep;
            ep = ctx_->ourRobot->measureLine();
            ed = ep - ed;
            currentOmega = ep*kp + ed*kd;
            currentVhorz = ep/8;
            // currentSpeed = currentSpeed + (speedSetpoint - ctx_->ourRobot->measSpeed)*kp;
            ctx_->ourRobot->omni4WD(-10, currentVhorz, currentOmega);
        }
    }
    ctx_->ourRobot->brake();
    delay(125);

    ctx_->ourRobot->move3DOF_heading(0, 500, -90);
    delay(125);
    ctx_->ourRobot->move3DOF_heading(0,-100,0);
    Serial.println("reading lever color");
    delay(125);
    // ctx_->ourRobot->omni4WD(vfwdSetpoint, 0, 0);
    // delay(1750);
    // ctx_->ourRobot->omni4WD(-vfwdSetpoint, 0, 0);
    // delay(100);
    // ctx_->ourRobot->omni4WD(0, 0, -64);
    // delay(750);
    // ctx_->ourRobot->omni4WD(vfwdSetpoint, 0, 0);
    // delay(250);
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
}

void StateButtonRead::update(){
    /*
    // ctx_->transitionTo(new StateHorizontalLine);
    int *colors = ctx_->ourRobot->readColors();
    float redSim = vDistance(colors, redAvg2);
    float greenSim = vDistance(colors, greenAvg2);
    float blueSim = vDistance(colors, blueAvg2);
    float yellowSim = vDistance(colors, yellowAvg);
    // float blackSim = vDistance(colors, blackAvg);
    // float whiteSim = vDistance(colors, whiteAvg);
    float leastDistance = min(min(redSim,yellowSim),min(blueSim, greenSim));

    if (leastDistance == redSim){
        Serial.println("Red");
        ctx_->transitionTo(new StateRed);
    }
    else if (leastDistance == greenSim){
        Serial.println("Green");
        ctx_->transitionTo(new StateGreen);
    }
    else if (leastDistance == blueSim){
        Serial.println("Blue");
        ctx_->transitionTo(new StateBlue);
    }
    else {
        Serial.println("Yellow");
        ctx_->transitionTo(new StateYellow);
    }
    //*/
}

void StateButtonRead::exit() {
    Serial.println("exiting state ButtonRead");
    ctx_->ourRobot->omni4WD(0, 150, 0);

    lastUpdate = micros();
    while (1) {
        newUpdate = micros();
        if (newUpdate - lastUpdate > ctx_->ourRobot->timestep) {
            ctx_->ourRobot->measureLine();
            if (ctx_->ourRobot->readBlackSenses() == 0)
                break;
        }
    }

    return;
}
