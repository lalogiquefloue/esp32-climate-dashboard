#include "HTTPManager.h"

HTTPManager::HTTPManager(int port) : server(port) {};
HTTPManager::~HTTPManager() {};

void HTTPManager::setupServer()
{
    server.on("/", HTTP_GET, [this]() {
        JsonDocument jsonDoc;
        jsonDoc["status"] = "success";
        jsonDoc["data"] = "Hello from ESP32";

        server.send(200, "application/json", serializeData(jsonDoc)); 
    });

    // Start the server
    server.begin();
    Serial.println("HTTP server started on ESP32");
}

void HTTPManager::handleClient()
{
    server.handleClient();
}

String HTTPManager::sendPostRequest(const String &url, const String &payload) 
{
    return "";
};

String HTTPManager::serializeData(JsonDocument data)
{
    String response;
    serializeJson(data, response);
    return response;
};
