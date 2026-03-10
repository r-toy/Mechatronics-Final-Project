#ifndef STATE_H
#define STATE_H

class Context;

class State {

    // pointer to Context class which we have not yet defined
    protected:
        Context* ctx_;

    // virtual functions to assign to specific states
    // since this is a template class
    public:
        virtual ~State() = default; // deconstructor
        void setContext(Context*);

        virtual void enter();
        virtual void update();
        virtual void exit();
};

#endif