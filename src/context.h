#ifndef CONTEXT_H
#define CONTEXT_H

class Robot;
class State;

class Context {
    private:
        State* curState_;

    public:
        Context(State*, Robot*);
        ~Context() = default;

        void transitionTo(State*);
        void update();

        Robot* ourRobot;

        // 1 = yellow, 2 = blue, 3 = red, 4 = green
        int color = 0;
        unsigned long globalTime = millis();
        // game over if millis() - globalTime > 180 000 ms
};

#endif