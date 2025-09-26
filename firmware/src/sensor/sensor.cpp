#include "sensor.h"

Sensor::Sensor()
    : dht(DHT_PIN, DHT_TYPE)
{
    dht.begin();
    Logging::Info("Sensor initialized...");
    temperature = 0.0f;
    humidity = 0.0f;
}

Sensor::~Sensor() {};

float Sensor::GetTemperature(){
    float t = dht.readTemperature();

    if (!isnan(t)) {
        temperature = t;
        Logging::Info("Temperature: " + std::to_string(temperature) + " °C");
    } else {
        Logging::Error("Failed to read temperature from DHT sensor");
    }

    return temperature;
};

float Sensor::GetHumidity(){
    float h = dht.readHumidity();

    if (!isnan(h)) {
        humidity = h;
        Logging::Info("Humidity: " + std::to_string(humidity) + " %");
    } else {
        Logging::Error("Failed to read humidity from DHT sensor");
    }

    return humidity;
};

std::string Sensor::dataToString() {
    GetHumidity();
    GetTemperature();
    return "Temperature: " + std::to_string(temperature) + " °C, " + "Humidity: " + std::to_string(humidity) + " %";
}