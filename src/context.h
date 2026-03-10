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
};

#endif