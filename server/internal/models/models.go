package models

type SensorData struct {
	Time        int64   `json:"time"`
	SensorID    string  `json:"sensorID"`
	Temperature float64 `json:"temperature"`
	Humidity    float64 `json:"humidity"`
}
