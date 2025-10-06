#ifndef HTTP_MANAGER_H
#define HTTP_MANAGER_H

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WebServer.h>

#include "config.h"
#include "sensor/sensor.h"
#include "utils/Timestamp.h"

class HTTPManager
{
private:
    WebServer server;
    void handleGetRootRequest();
    void handleGetDataRequest();
    String serializeData(JsonDocument data);
    
    public:
    HTTPManager(int port = 80);
    ~HTTPManager();
    
    void setupServer();
    void handleClient();
    void handlePostDataToServer(const String &url, const JsonDocument &payload);
};

#endif
