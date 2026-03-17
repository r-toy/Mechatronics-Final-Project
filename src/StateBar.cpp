#include <Arduino.h>
#include "robot.h"
#include "context.h"
#include "state.h"
#include "StateButtonRead.h"

float vDistance(const int *one, const int *two){
  int i = 0, dsqr = 0;
  for (i; i < 3; i++) {
    dsqr += (one[i] - two[i]) * (one[i] - two[i]);
  }
  return sqrt((double)(dsqr));
}

void StateButtonRead::enter(){
    Serial.println("entering state Bar");
    ctx_->ourRobot->move3DOF_heading(0, 400, 0);
    ctx_->ourRobot->servoPosition(90);
    ctx_->ourRobot->move3DOF_heading(0, -100, 0);

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
        Serial.println("GREEN");
        ctx_->transitionTo(new StateGreen);
    else if (leastDistance == blueSim)
        Serial.println("BLUE");
        ctx_->transitionTo(new StateBlue);
    else
        Serial.println("YELLOW");
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
