#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <WiFi.h>

#include "config.h"
#include "logging/Logging.h"

class NetworkManager {
private:
    String ssid;
    String password;
    String localIp;

public:
    NetworkManager();
    ~NetworkManager();

    WiFiClient client;

    void connect();
    void verifyConnection();
    bool isConnected();
};

#endif

