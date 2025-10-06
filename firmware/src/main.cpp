#include "config.h"
#include "http_manager/HTTPManager.h"
#include "network_manager/NetworkManager.h"
#include "sensor/sensor.h"
#include "utils/Timestamp.h"

const String BASE_URL = String("http://") + SERVER_URL + String(":") + String(SERVER_PORT);

NetworkManager networkManager;
HTTPManager httpManager(LOCAL_PORT);

void setup()
{
  Logging::begin();
  networkManager.connect();
  Timestamp::initialize();
  httpManager.setupServer();
  sensor.initialize();
}

void loop()
{
  delay(UPDATE_RATE);

  httpManager.handleClient();

  sensor.updateSensorData();

  JsonDocument sensorData = sensor.dataToJson();
  httpManager.handlePostDataToServer(BASE_URL + "/sensor/data", sensorData);
}