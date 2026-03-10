#include "state.h"

// only need to define setState because other functions will be defined by children
void State::setContext(Context* ctx) {
    ctx_ = ctx;
}