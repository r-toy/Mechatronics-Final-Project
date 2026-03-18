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

float StateButtonRead::cosineSim(int *one, int *two){
  int dot = 0;
  double magOne = 0, magTwo = 0;
  for (int i = 0; i < 3; i++) {
    dot += one[i]*two[i];
    magOne += one[i]*one[i];
    magTwo += two[i]*two[i];
  }
  magOne = sqrt(magOne);
  magTwo = sqrt(magTwo);
  return dot / (magOne * magTwo);
}


void StateButtonRead::enter(){
    Serial.println("entering state ButtonRead");
    ctx_->ourRobot->move3DOF_heading(300, 0, 0, &Robot::defaultEndcon, 255);
    delay(125);
    Serial.println("button pressed");
    ctx_->ourRobot->move3DOF_heading(0, 120, -85);
    delay(125);
    Serial.println("back up");
    // ctx_->ourRobot->move3DOF_heading(0, -100, 0);
    delay(125);
    Serial.println("reading lever");
    // ctx_->ourRobot->omni4WD(vfwdSetpoint, 0, 0);
    // delay(1750);
    // ctx_->ourRobot->omni4WD(-vfwdSetpoint, 0, 0);
    // delay(100);
    // ctx_->ourRobot->omni4WD(0, 0, -64);
    // delay(750);
    // ctx_->ourRobot->omni4WD(vfwdSetpoint, 0, 0);
    // delay(250);

}

void StateButtonRead::update(){
    // ctx_->transitionTo(new StateHorizontalLine);
    long red = 0, green = 0, blue = 0;
    long * colors;
    for (int i = 0; i < 32; i++){
        ctx_->ourRobot->senseColor();
        colors = ctx_->ourRobot->readColors();

        red += colors[0];
        green += colors[1];
        blue += colors[2];
    }

    red /=32;
    green /= 32;
    blue /= 32;

    Serial.print("Red: "); Serial.print(red);
    Serial.print(" Green: "); Serial.print(green);
    Serial.print(" Blue: "); Serial.println(blue);

    // float redSim = vDistance(colors, redAvg2);
    // float greenSim = vDistance(colors, greenAvg2);
    // float blueSim = vDistance(colors, blueAvg2);
    // float yellowSim = vDistance(colors, yellowAvg);
    // float blackSim = vDistance(colors, blackAvg);
    // float whiteSim = vDistance(colors, whiteAvg);
    // float leastDistance = min(min(redSim,yellowSim),min(blueSim, greenSim));

    // if (leastDistance == redSim){
    //     Serial.println("Red");
    //     ctx_->transitionTo(new StateRed);
    // }
    // else if (leastDistance == greenSim){
    //     Serial.println("Green");
    //     ctx_->transitionTo(new StateGreen);
    // }
    // else if (leastDistance == blueSim){
    //     Serial.println("Blue");
    //     ctx_->transitionTo(new StateBlue);
    // }
    // else {
    //     Serial.println("Yellow");
    //     ctx_->transitionTo(new StateYellow);
    // }
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
