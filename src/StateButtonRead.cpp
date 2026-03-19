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

long StateButtonRead::cosineSim(long *one, long *two){
  long dot = 0;

  for (int i = 0; i < 3; i++) {
    dot += one[i]*two[i];
  }
  return dot;
}


void StateButtonRead::enter(){
    Serial.println("entering state ButtonRead");
    ctx_->ourRobot->move3DOF_heading(320, 0, 0, &Robot::defaultEndcon, 255);
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

    ctx_->ourRobot->move3DOF_heading(0, 110, -90);
    delay(125);
    ctx_->ourRobot->move3DOF_heading(0,-50,0);
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
    // while (1) {
    //     unsigned short colors[4];
    //     unsigned short red = 0, green = 0, blue = 0, clear = 0;
    //     ctx_->ourRobot->senseColor();
    //     ctx_->ourRobot->readColors(colors);
    //     for (int i = 0; i < 64; i++){
    //         red += colors[0];
    //         green += colors[1];
    //         blue += colors[2];
    //         clear += colors[3];
    //     }
    //     Serial.print("red: "); Serial.print(red/64);
    //     Serial.print(" green: "); Serial.print(green/64);
    //     Serial.print(" blue: "); Serial.print(blue/64);
    //     Serial.print(" clear: "); Serial.println(clear/64);
    //     delay(1000);
    // }
}

void StateButtonRead::update(){
    //*
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
    red /= 64;
    green /= 64;
    blue /= 64;
    clear /= 64;
    long meas[4] = {red,green,blue,clear};
    long redSim = cosineSim(meas, redRef2);
    long greenSim = cosineSim(meas, greenRef2);
    long blueSim = cosineSim(meas, blueRef2);
    long yellowSim = cosineSim(meas, yellowRef2);

    long leastDistance = max(max(redSim,yellowSim),max(blueSim, greenSim));
    Serial.print(meas[0]); Serial.print(" "); Serial.print(meas[1]); Serial.print(" "); Serial.println(meas[2]);

    if (leastDistance == redSim){
        Serial.println("Red");
        ctx_->color = 3;
        // ctx_->transitionTo(new StateRed);
    }
    else if (leastDistance == blueSim){
        Serial.println("Blue");
        ctx_->color = 2;
        // ctx_->transitionTo(new StateRed);
    }
    // either green or yellow
    else if (blueSim > redSim){
        Serial.println("Green");
        ctx_->color = 4;
        // ctx_->transitionTo(new StateGreen);
    }
    else {
        Serial.println("Yellow");
        ctx_->color = 1;
        // ctx_->transitionTo(new StateYellow);
    }
    ctx_->transitionTo(new StateColor);
    //*/
}

void StateButtonRead::exit() {
    Serial.println("exiting state ButtonRead");
    ctx_->ourRobot->move3DOF_heading(-100, 50, 0);
    ctx_->ourRobot->omni4WD(0, 150, 0);

    lastUpdate = micros();
    while (1) {
        newUpdate = micros();
        if (newUpdate - lastUpdate > ctx_->ourRobot->timestep) {
            ctx_->ourRobot->measureLine();
            // Serial.print("blackSenses"); Serial.println(ctx_->ourRobot->readBlackSenses());
            if (ctx_->ourRobot->readBlackSenses() != 0)
                break;
        }
    }

    return;
}
