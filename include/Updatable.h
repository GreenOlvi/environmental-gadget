#ifndef Updatable_h
#define Updatable_h

class Updatable {
    public:
        virtual void update(unsigned long t) = 0;
};

#endif