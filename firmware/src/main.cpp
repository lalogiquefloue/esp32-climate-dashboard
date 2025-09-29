#include "HTTPManager/HTTPManager.h"
#include "NetworkManager/NetworkManager.h"
#include "Sensor/Sensor.h"
#include "config.h"

Sensor sensor;
NetworkManager networkManager;
HTTPManager httpManager(LOCAL_PORT);

void setup()
{
  Logging::begin();
  networkManager.connect();
  httpManager.setupServer();
  sensor.Initialize();
}

void loop()
{
  delay(UPDATE_RATE);
  sensor.UpdateSensorData();
  httpManager.handleClient();
}