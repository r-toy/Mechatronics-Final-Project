#include <Arduino.h>
#include "context.h"
#include "StateInit.h"
#include "robot.h"

Context *ctx;

void setup() {
  ctx = new Context(new StateInit, new Robot);

}

void loop() {
  ctx->update();
}

void isrFLDistance() {
  ctx->ourRobot->FLDistance();
}

void isrFRDistance() {
  ctx->ourRobot->FRDistance();

}void isrBLDistance() {
  ctx->ourRobot->BLDistance();
}

void isrBRDistance() {
  ctx->ourRobot->BRDistance();
}