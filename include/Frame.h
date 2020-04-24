#ifndef Frame_h
#define Frame_h

#include <SSD1306.h>

class Frame {
    public:
        virtual void draw(OLEDDisplay* display) = 0;
};

#endif