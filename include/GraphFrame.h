#ifndef GraphFrame_h
#define GraphFrame_h

#include "Frame.h"
#include "WindowedStack.h"

class GraphFrame : public Frame {
    public:
        GraphFrame(WindowedStack* graphData);
        virtual void draw(OLEDDisplay* display);
    private:
        WindowedStack* _graphData;
};

class TemperatureGraphFrame : public GraphFrame {
    public:
        TemperatureGraphFrame(WindowedStack* graphData, float* temperature);
        TemperatureGraphFrame(WindowedStack* graphData, float* temperature, const char* line2);
        virtual void draw(OLEDDisplay* display);
    private:
        float* _temperature;
        const char* _line2;
};

class TemperatureHumidityGraphFrame : public GraphFrame {
    public:
        TemperatureHumidityGraphFrame(WindowedStack* graphData, float* temperature, float* humidity);
        virtual void draw(OLEDDisplay* display);
    private:
        float* _temperature;
        float* _humidity;
};

#endif