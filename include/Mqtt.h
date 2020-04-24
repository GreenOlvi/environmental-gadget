#ifndef Mqtt_h
#define Mqtt_h

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define RECONNECT_DELAY 1000

class MqttClient {
    public:
        MqttClient(const char* clientId, const char* hostname, unsigned short port = 1883);
        bool isConnected();
        void update(unsigned long t);
        void publish(const char* topic, const char* payload);
        void publish(const char* type, float value);
    private:
        PubSubClient _client;
        const char* _clientId;
        unsigned long _lastUpdate = 0;
        void callback(char* topic, byte* payload, unsigned int length);
};

#endif