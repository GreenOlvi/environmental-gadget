#ifndef GraphFrame_h
#define GraphFrame_h

#include "SSD1306.h"
#include "Graph.h"
#include "Frame.h"
#include "RollingBuffer.h"

class GraphFrame : public Frame {
    public:
        GraphFrame(RollingBuffer* graphData);
        virtual void draw(OLEDDisplay* display) override;
    private:
        const RollingBuffer* _graphData;
};

class TemperatureGraphFrame : public GraphFrame {
    public:
        TemperatureGraphFrame(RollingBuffer* graphData, float* temperature);
        TemperatureGraphFrame(RollingBuffer* graphData, float* temperature, const char* line2);
        virtual void draw(OLEDDisplay* display) override;
    private:
        const float* _temperature;
        const char* _line2;
};

class TemperatureHumidityGraphFrame : public GraphFrame {
    public:
        TemperatureHumidityGraphFrame(RollingBuffer* graphData, float* temperature, float* humidity);
        virtual void draw(OLEDDisplay* display) override;
    private:
        const float* _temperature;
        const float* _humidity;
};

#endif