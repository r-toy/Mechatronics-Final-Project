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

void leftISR() {
  ctx->ourRobot->LeftDistance();
}

void rightISR() {
  ctx->ourRobot->RightDistance();
}