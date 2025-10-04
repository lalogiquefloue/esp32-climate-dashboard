#include "sensor.h"

Sensor sensor;

Sensor::Sensor() : dht(DHT_PIN, DHT_TYPE){};
Sensor::~Sensor() {};

void Sensor::Initialize(){
    sensorID = ESP.getEfuseMac();
    dht.begin();
    
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    update_time = Timestamp::GetCurrentTime();
    temperature = GetTemperature();
    humidity = GetHumidity();

    Logging::Info("Sensor initialized...");
}

float Sensor::GetTemperature(){
    float t = dht.readTemperature();

    if (!isnan(t)) {
        temperature = t;
    } else {
        Logging::Error("Failed to read temperature from DHT sensor...");
    }

    return temperature;
};

float Sensor::GetHumidity(){
    float h = dht.readHumidity();

    if (!isnan(h)) {
        humidity = h;
    } else {
        Logging::Error("Failed to read humidity from DHT sensor...");
    }

    return humidity;
};

void Sensor::UpdateSensorData(){
    GetHumidity();
    GetTemperature();

    time_t now = time(nullptr);
    if (now > 0) {
        update_time = now;
    } else {
        Logging::Error("Failed to get current time...");
    }
};

String Sensor::dataToString() {
    return "Sensor ID: " + String(sensorID) + ", Time:" + String(update_time) + ", Temperature: " + String(temperature) + " °C, " + "Humidity: " + String(humidity) + " %";
}

JsonDocument Sensor::dataToJson() {
    JsonDocument data;

    data["sensorID"] = String(sensorID);
    data["time"] = update_time;
    data["temperature"] = temperature;
    data["humidity"] = humidity;

    return data;
}