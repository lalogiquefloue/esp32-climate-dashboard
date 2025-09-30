#include "HTTPManager.h"
#include "../Sensor/sensor.h"

HTTPManager::HTTPManager(int port) : server(port) {};
HTTPManager::~HTTPManager() {};

void HTTPManager::setupServer()
{
    server.on("/", HTTP_GET, [this]() { handleGetRootRequest(); });
    server.on("/sensor/data", HTTP_GET, [this]() { handleGetDataRequest(); });
    // server.on("/sensor/id", HTTP_GET, [this]() {});
    // server.on("/sensor/ip", HTTP_GET, [this]() {});

    // Start the server
    server.begin();
    Serial.println("HTTP server started on ESP32");
}

void HTTPManager::handleClient()
{
    server.handleClient();
}

void HTTPManager::handleGetRootRequest()
{
    JsonDocument jsonData;
    jsonData["status"] = "success";
    jsonData["data"] = "Hello from ESP32";
    server.send(200, "application/json", serializeData(jsonData));
}

void HTTPManager::handleGetDataRequest()
{
    sensor.UpdateSensorData();
    JsonDocument jsonData = sensor.dataToJson();
    server.send(200, "application/json", serializeData(jsonData));
}

void HTTPManager::handlePostDataToServer(const String &url, const JsonDocument &payload) 
{
    HTTPClient http;
    http.begin(url);  
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(serializeData(payload));
    
    if (httpResponseCode > 0) {
        String response = http.getString();
        Logging::Info("POST Response: " + response);
    } else {
        Logging::Error("POST failed, error: " + String(httpResponseCode));
    }

    http.end();
}


String HTTPManager::serializeData(JsonDocument data)
{
    String response;
    serializeJson(data, response);
    return response;
}
