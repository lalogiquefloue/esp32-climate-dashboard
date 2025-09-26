#ifndef SENSOR_H
#define SENSOR_H

#include <DHT.h>
#include "config.h"
#include "logging/logging.h"

class Sensor {
    private:
        int sensorID;

    public:
        Sensor();
        ~Sensor();

        float GetTemperature();
        float GetHumidity();

        void SendData();
};

#endif