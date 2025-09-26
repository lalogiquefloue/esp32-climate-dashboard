#include "NetworkManager.h"

WiFiClient client;

NetworkManager::NetworkManager() {
    this->ssid = WIFI_SSID;
    this->password = WIFI_PASSWORD;
}

NetworkManager::~NetworkManager() {
    WiFi.disconnect();
}

void NetworkManager::connect() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        Logging::Info("Waiting for WiFi connection...");
        delay(1000);
    }
}

void NetworkManager::verifyConnection(){
    if (WiFi.status() != WL_CONNECTED) {
        Logging::Info("Reconnecting to WiFi...");
        WiFi.disconnect();
        connect();
    }
}

bool NetworkManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}