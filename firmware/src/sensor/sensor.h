#ifndef SENSOR_H
#define SENSOR_H

#include <DHT.h>
#include "config.h"
#include "logging/logging.h"

class Sensor {
    private:
        // int sensorID; // TODO: how should this be initialised
        DHT dht;
        float humidity;
        float temperature;

    public:
        Sensor();
        ~Sensor();

        float GetTemperature();
        float GetHumidity();

        std::string dataToString();
};

#endif