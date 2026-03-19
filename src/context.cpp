#include "context.h"
#include "state.h"
#include "robot.h"

// class creator, initialize internal variable as null pointer
Context::Context(State* initialState, Robot* newRobot) : curState_(nullptr) {
    globalTime = millis();
    ourRobot = newRobot;
    transitionTo(initialState);
}

void Context::transitionTo(State* newState) {
    if (curState_ != nullptr) {
        // exit current state
        curState_ -> exit();
        delete curState_;
    }

    // enter new state
    curState_ = newState;
    curState_ -> setContext(this);
    curState_ -> enter();
}

void Context::update() {
    curState_->update();
}