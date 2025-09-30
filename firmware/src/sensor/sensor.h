#ifndef SENSOR_H
#define SENSOR_H

#include <ArduinoJson.h>
#include <DHT.h>
#include "config.h"
#include "logging/logging.h"

class Sensor {
    private:
        DHT dht;
        uint64_t sensorID;
        float humidity;
        float temperature;
        int update_time;
        
    public:
        Sensor();
        ~Sensor();
        
        void Initialize();

        float GetTemperature();
        float GetHumidity();
        void UpdateSensorData();

        String dataToString();
        JsonDocument dataToJson();
};

extern Sensor sensor;

#endif