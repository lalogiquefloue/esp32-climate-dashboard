#ifndef SENSOR_H
#define SENSOR_H

#include <ArduinoJson.h>
#include <DHT.h>

#include "config.h"
#include "logging/Logging.h"
#include "utils/Timestamp.h"

class Sensor {
    private:
        DHT dht;
        uint64_t sensorID;
        time_t updateTime;
        float humidity;
        float temperature;
        
    public:
        Sensor();
        ~Sensor();
        
        void initialize();

        float getTemperature();
        float getHumidity();
        void updateSensorData();

        String dataToString();
        JsonDocument dataToJson();
};

extern Sensor sensor;

#endif