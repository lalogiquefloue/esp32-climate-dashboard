#ifndef CONFIG_H
#define CONFIG_H

#define DHT_PIN 4
#define DHT_TYPE DHT22

#define BAUD 115200

#define WIFI_SSID  ""
#define WIFI_PASSWORD ""

#define LOCAL_PORT 9876

#define SERVER_URL "192.168.0.2"
#define SERVER_PORT 8765

#define MAX_LOGS 10 // number of logs kept in memory

#define UPDATE_RATE 10000 // time delta between sensor readings in ms 

#define TIME_ZONE_SPECIFICATION "EST5EDT,M3.2.0,M11.1.0" // https://di-mgt.com.au/wclock/help/wclo_tzexplain.html

#endif