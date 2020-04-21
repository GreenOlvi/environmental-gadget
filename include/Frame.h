#ifndef Frame_h
#define Frame_h

#include <SSD1306.h>

class Frame
{
    public:
        Frame();
        virtual void draw(OLEDDisplay* display);
};

#endif