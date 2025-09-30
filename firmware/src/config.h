#ifndef CONFIG_H
#define CONFIG_H

#define DHT_PIN 4
#define DHT_TYPE DHT22

#define BAUD 115200

#define WIFI_SSID  ""
#define WIFI_PASSWORD ""

#define LOCAL_PORT 8765

#define SERVER_URL "192.168.0.22"
#define SERVER_PORT 5000

#define MAX_LOGS 10 // number of logs kept in memory

#define UPDATE_RATE 1000 // time delta between sensor readings in ms 

#endif