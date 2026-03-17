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
    ctx_->ourRobot->move3DOF_heading(300, 0, 0, &Robot::defaultEndcon,255);
    ctx_->ourRobot->move3DOF_heading(-20, 25, -45);
    ctx_->ourRobot->move3DOF_heading(20, 25, -45);
    // ctx_->ourRobot->omni4WD(vfwdSetpoint, 0, 0);
    // delay(1750);
    // ctx_->ourRobot->omni4WD(-vfwdSetpoint, 0, 0);
    // delay(100);
    // ctx_->ourRobot->omni4WD(0, 0, -64);
    // delay(750);
    // ctx_->ourRobot->omni4WD(vfwdSetpoint, 0, 0);
    // delay(250);
    while (ctx_->ourRobot->colorDetect())
        ctx_->ourRobot->senseColor();

}

void StateButtonRead::update(){
    // ctx_->transitionTo(new StateHorizontalLine);
    const int *colors = ctx_->ourRobot->readColors();
    float redSim = vDistance(colors, redAvg2);
    float greenSim = vDistance(colors, greenAvg2);
    float blueSim = vDistance(colors, blueAvg2);
    float yellowSim = vDistance(colors, yellowAvg);
    // float blackSim = vDistance(colors, blackAvg);
    // float whiteSim = vDistance(colors, whiteAvg);
    float leastDistance = min(min(redSim,yellowSim),min(blueSim, greenSim));

    if (leastDistance == redSim)
        ctx_->transitionTo(new StateRed);
    else if (leastDistance == greenSim)
        ctx_->transitionTo(new StateGreen);
    else if (leastDistance == blueSim)
        ctx_->transitionTo(new StateBlue);
    else
        ctx_->transitionTo(new StateYellow);

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
