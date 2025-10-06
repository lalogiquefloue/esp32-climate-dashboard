#include "sensor.h"

Sensor sensor;

Sensor::Sensor() : dht(DHT_PIN, DHT_TYPE){};
Sensor::~Sensor() {};

void Sensor::initialize(){
    sensorID = ESP.getEfuseMac();
    dht.begin();
    
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    updateTime = Timestamp::getCurrentTime();
    temperature = getTemperature();
    humidity = getHumidity();

    Logging::info("Sensor initialized...");
}

float Sensor::getTemperature(){
    float t = dht.readTemperature();

    if (!isnan(t)) {
        temperature = t;
    } else {
        Logging::error("Failed to read temperature from DHT sensor...");
    }

    return temperature;
};

float Sensor::getHumidity(){
    float h = dht.readHumidity();

    if (!isnan(h)) {
        humidity = h;
    } else {
        Logging::error("Failed to read humidity from DHT sensor...");
    }

    return humidity;
};

void Sensor::updateSensorData(){
    getHumidity();
    getTemperature();

    time_t now = time(nullptr);
    if (now > 0) {
        updateTime = now;
    } else {
        Logging::error("Failed to get current time...");
    }
};

String Sensor::dataToString() {
    return "Sensor ID: " + String(sensorID) + ", Time:" + String(updateTime) + ", Temperature: " + String(temperature) + " °C, " + "Humidity: " + String(humidity) + " %";
}

JsonDocument Sensor::dataToJson() {
    JsonDocument data;

    data["sensorID"] = String(sensorID);
    data["time"] = updateTime;
    data["temperature"] = temperature;
    data["humidity"] = humidity;

    return data;
}