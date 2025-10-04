#ifndef SENSOR_H
#define SENSOR_H

#include <ArduinoJson.h>
#include <DHT.h>
#include "config.h"
#include "logging/logging.h"
#include "utils/Timestamp.h"

class Sensor {
    private:
        DHT dht;
        uint64_t sensorID;
        time_t update_time;
        float humidity;
        float temperature;
        
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