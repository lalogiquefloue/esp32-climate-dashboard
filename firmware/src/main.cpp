#include "HTTPManager/HTTPManager.h"
#include "NetworkManager/NetworkManager.h"
#include "Sensor/Sensor.h"
#include "utils/Timestamp.h"
#include "config.h"

NetworkManager networkManager;
HTTPManager httpManager(LOCAL_PORT);
String BASE_URL = String("http://") + SERVER_URL + String(":") + String(SERVER_PORT);

void setup()
{
  Logging::begin();
  networkManager.connect();
  Timestamp::Initialize();
  httpManager.setupServer();
  sensor.Initialize();
}

void loop()
{
  delay(UPDATE_RATE);

  httpManager.handleClient();

  sensor.UpdateSensorData();

  JsonDocument sensorData = sensor.dataToJson();
  httpManager.handlePostDataToServer(BASE_URL + "/sensor/data", sensorData);
}