#ifndef NETWORK_H
#define NETWORK_H

#include <WiFi.h>
#include <Arduino.h>
#include "config.h"
#include "../Logging/Logging.h"

class NetworkManager {
private:
    String ssid;
    String password;

public:
    NetworkManager();
    ~NetworkManager();

    WiFiClient client;

    void connect();
    void verifyConnection();
    bool isConnected();
    // std::string getIP();
};

#endif

