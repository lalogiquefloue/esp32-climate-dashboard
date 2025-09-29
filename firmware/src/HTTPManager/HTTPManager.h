#ifndef HTTP_MANAGER_H
#define HTTP_MANAGER_H

#include "../config.h"
#include <ArduinoJson.h>
#include <WebServer.h>
#include <string>

class HTTPManager
{
private:
    WebServer server;

public:
    HTTPManager(int port = 80);
    ~HTTPManager();

    void setupServer();
    void handleClient();
    String sendPostRequest(const String &url, const String &payload);
    String serializeData(JsonDocument data);
};

#endif
