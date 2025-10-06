#include "NetworkManager.h"

WiFiClient client;

NetworkManager::NetworkManager()
{
    this->ssid = WIFI_SSID;
    this->password = WIFI_PASSWORD;
}

NetworkManager::~NetworkManager()
{
    WiFi.disconnect();
}

void NetworkManager::connect()
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        Logging::info("Waiting for WiFi connection...");
        delay(1000);
    }

    localIp = WiFi.localIP().toString();
    Logging::info("Connected to Wifi, local IP address: " + localIp);
}

void NetworkManager::verifyConnection()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Logging::info("Reconnecting to WiFi...");
        WiFi.disconnect();
        connect();
    }
}

bool NetworkManager::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}