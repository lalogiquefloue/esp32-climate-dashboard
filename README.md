# esp32-climate-dashboard

## Project Overview

The ESP32 Climate Dashboard is a system made to monitor and visualize sensor data collected by a ESP32 based sensor node. 

Personal note: 
The project originated from the need to track humidity for musical instruments protection so I can be able to access this data on my local network and remotely using a VPN. I chose to use the Go programming langage for the backend so I could introduce myself to a new langage.

### System Architecture
- Client (ESP32 firmware): Reads data from a DHT sensor and posts data to the backend server for persistence.
- Server (Backend with WebUI): Receives data from the sensors for persistence in a databse and serves the WebUI for visualizing this data. Realtime data can be fetched from the `GET /api/sensor/latest` endpoint and historical data can be fetched from the `GET /api/sensor/range?range={range}&SensorID={SensorID}` endpoint.

### Technology stack
| Layer        | Technology                        | Description                                                                                               |
| ------------ | --------------------------------- | --------------------------------------------------------------------------------------------------------- |
| **Firmware** | ESP32, C++ (Arduino framework)    | Runs on the ESP32 microcontroller and handles sensor reading, Wi-Fi, and HTTP communication to the server |
| **Sensor**   | DHT22                             | Measures temperature and humidity                                                                         |
| **Backend**  | Go (Golang)                       | REST API and data ingestion                                                                               |
| **Database** | InfluxDB                          | Time-series database                                                                                      |
| **Frontend** | React + Vite + TypeScript + D3.js | Web dashboard for data visualization                                                                      |

## Firmware

The firmware source is under `/firmware/src`.

### API Endpoints
| Endpoint     | Method | Description                     | Parameters | Sample Response                                                                      | Errors |
| ------------ | ------ | ------------------------------- | ---------- | ------------------------------------------------------------------------------------ | ------ |
| /            | GET    | Returns firmware server status. | None       | `{"status": "success", "data": "Hello from ESP32"}`                                  | TODO   |
| /sensor/data | GET    | Returns latest sensor readings. | None       | `{"sensorID": "sensorID", "time": 123456789, "temperature": 20.0, "humidity": 50.0}` | TODO   |

## Server / WebUI

The server backend handles data ingestion and provides visualization tools.
The React WebUI lives under `/server/webui`.

### API Endpoints
| Endpoint           | Method | Description                                                            | Parameters                                                                                                                                             | Sample Response                                                                                                  | Errors |
| ------------------ | ------ | ---------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------------------------------------------------- | ------ |
| /api/sensor/data   | POST   | Receives latest sensor data from the ESP32 firmware for ingestion.     | None                                                                                                                                                   | `{ "status": "ok", "message": "data received" }`                                                                 | TODO   |
| /api/sensor/latest | GET    | Retrieves the single latest sensor data point for the real-time table. | None                                                                                                                                                   | `{"data":{"time":1760982283,"sensorID":"44287646293408","temperature":21.5,"humidity":59},"status":"success"}`   | TODO   |
| /api/sensor/range  | GET    | Retrieves historical data points for a specified time range.           | `range` (optional, defaults to 1h): Time duration (influxDB format: 1m, 1h, 1d). `sensorID` (optional, if None returns all sensors): Filter by sensor. | `{"count":295,"data":[{"time":1760979301,"sensorID":"44287646293408","temperature":21.3,"humidity":58.7}, ...]}` | TODO   |

## Setup and configuration

TODO

## Future Development and TODOs:

#### Firmware
- API endpoint to access last registered logs
- Find a way to be able to modify sensor parameters remotely without hardcoding (we should be able to send requests to the microcontroller, maybe they should broadcast their networking infos and it would be stored securely on the server and managed through the dashboard?)

#### Server
- Go server to InfluxDB connection loop instead of crash
- Support for multiple sensors (other types of data and multiple sensors of the same kind)
- Implement authentification
- Add more specific error codes and messaging to the API

#### UI
- Reformat UI so it is more readable on mobile, see if there is a way to make it dynamic
- Android/IOS clients
- Implement SensorID selection in the WebUI
- Moving average on data to smooth the visualization