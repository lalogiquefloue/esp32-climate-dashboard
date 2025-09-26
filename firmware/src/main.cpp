#include "NetworkManager/NetworkManager.h"
#include "Sensor/Sensor.h"
#include "config.h"

Sensor sensor;
NetworkManager networkManager;

void setup() {
  Logging::begin();
  networkManager.connect();
}

void loop() {
  delay(UPDATE_RATE);
  Logging::Info(networkManager.isConnected() ? "Connected" : "Not connected");
  sensor.GetHumidity();
  sensor.GetTemperature();
}