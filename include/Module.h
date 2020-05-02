#ifndef Module_h
#define Module_h

#include "Updatable.h"

class Module : public Updatable {
    public:
        virtual void setup() = 0;
        virtual void update(unsigned long t) = 0;
};

#endif