#include "SSD1306.h"
#include "GraphFrame.h"
#include "Graph.h"

GraphFrame::GraphFrame(WindowedStack* graphData) {
    _graphData = graphData;
}

void GraphFrame::draw(OLEDDisplay* display) {
    drawGraph(display, 0, 0, 62, 32, _graphData->getData(), _graphData->Count());
}

TemperatureGraphFrame::TemperatureGraphFrame(WindowedStack* graphData, float* temperature) : GraphFrame(graphData) {
    _temperature = temperature;
    _line2 = "";
}

TemperatureGraphFrame::TemperatureGraphFrame(WindowedStack* graphData, float* temperature, const char* line2) : GraphFrame(graphData) {
    _temperature = temperature;
    _line2 = line2;
}

void TemperatureGraphFrame::draw(OLEDDisplay* display) {
    GraphFrame::draw(display);

    display->setTextAlignment(OLEDDISPLAY_TEXT_ALIGNMENT::TEXT_ALIGN_RIGHT);
    display->setFont(ArialMT_Plain_16);

    char line1[10];
    sprintf(line1, "%.1f°C", *_temperature);
    display->drawString(128, 0, line1);

    display->drawString(128, 16, _line2);
}

TemperatureHumidityGraphFrame::TemperatureHumidityGraphFrame(WindowedStack* graphData, float* temperature, float* humidity) : GraphFrame(graphData) {
    _temperature = temperature;
    _humidity = humidity;
}

void TemperatureHumidityGraphFrame::draw(OLEDDisplay* display) {
    GraphFrame::draw(display);

    display->setTextAlignment(OLEDDISPLAY_TEXT_ALIGNMENT::TEXT_ALIGN_RIGHT);
    display->setFont(ArialMT_Plain_16);

    char line1[10];
    sprintf(line1, "%.1f°C", *_temperature);
    display->drawString(128, 0, line1);

    char line2[4];
    sprintf(line2, "%.0f%%", *_humidity);
    display->drawString(128, 16, line2);
}