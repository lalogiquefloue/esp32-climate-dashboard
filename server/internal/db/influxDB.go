package db

import (
	"context"
	"fmt"
	"log"
	"server/internal/configs"
	"server/internal/models"

	"time"

	influxdb2 "github.com/influxdata/influxdb-client-go/v2"
	"github.com/influxdata/influxdb-client-go/v2/api"
)

const MEASUREMENT_NAME = "sensor-data"

type InfluxDB struct {
	Client   influxdb2.Client
	WriteAPI api.WriteAPIBlocking
	QueryAPI api.QueryAPI
	Config   *configs.Config
}

func NewInfluxDB(config *configs.Config) *InfluxDB {
	fmt.Println("Waiting 10 seconds for InfluxDB to be ready...")
	time.Sleep(10 * time.Second)

	client := influxdb2.NewClient(config.Database.URL, config.Database.Token)
	health, err := client.Health(context.Background())

	if err != nil || health.Status != "pass" {
		log.Fatalf("InfluxDB not healthy: %+v", health)
	}

	return &InfluxDB{
		Client:   client,
		WriteAPI: client.WriteAPIBlocking(config.Database.Org, config.Database.Bucket),
		QueryAPI: client.QueryAPI(config.Database.Org),
		Config:   config,
	}
}

func (db *InfluxDB) WriteSensorData(sensorID string, temp, humidity float64, timestamp time.Time) error {
	p := influxdb2.NewPointWithMeasurement(MEASUREMENT_NAME).
		AddTag("sensorID", sensorID).
		AddField("temperature", temp).
		AddField("humidity", humidity).
		SetTime(timestamp)

	return db.WriteAPI.WritePoint(context.Background(), p)
}

func (db *InfluxDB) QueryRecentSensorData(sensorID string, duration string) ([]models.SensorData, error) {
	if duration == "" {
		duration = "1h" // default to 1 hour if no duration or invalid duration is provided
	}

	// Build Flux query
	var query string
	if sensorID == "" {
		// No sensor specified: return all sensors
		query = fmt.Sprintf(`
		from(bucket: "%s")
			|> range(start: -%s)
			|> filter(fn: (r) => r._measurement == "%s")
			|> filter(fn: (r) => r._field == "temperature" or r._field == "humidity")
			|> pivot(rowKey:["_time"], columnKey:["_field"], valueColumn:"_value")
			|> keep(columns: ["_time", "sensorID", "temperature", "humidity"])
	`, db.Config.Database.Bucket, duration, MEASUREMENT_NAME)
	} else {
		query = fmt.Sprintf(`
		from(bucket: "%s")
			|> range(start: -%s)
			|> filter(fn: (r) => r._measurement == %s and r.sensorID == "%s")
			|> filter(fn: (r) => r._field == "temperature" or r._field == "humidity")
			|> pivot(rowKey:["_time"], columnKey:["_field"], valueColumn:"_value")
			|> keep(columns: ["_time", "sensorID", "temperature", "humidity"])
	`, db.Config.Database.Bucket, duration, MEASUREMENT_NAME, sensorID)
	}

	result, err := db.QueryAPI.Query(context.Background(), query)
	if err != nil {
		return nil, err
	}

	defer result.Close()

	var data []models.SensorData

	for result.Next() {
		values := result.Record().Values()

		temperature, _ := values["temperature"].(float64)
		humidity, _ := values["humidity"].(float64)
		time, _ := values["_time"].(time.Time)
		sensorID, _ := values["sensorID"].(string)

		data = append(data, models.SensorData{
			Time:        time.Unix(),
			SensorID:    sensorID,
			Temperature: temperature,
			Humidity:    humidity,
		})
	}

	if result.Err() != nil {
		return nil, result.Err()
	}

	return data, nil
}

func (db *InfluxDB) Close() {
	db.Client.Close()
}
