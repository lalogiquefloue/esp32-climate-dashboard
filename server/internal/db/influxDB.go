package db

import (
	"context"
	"fmt"
	"log"
	"server/internal/configs"
	"time"

	influxdb2 "github.com/influxdata/influxdb-client-go/v2"
	"github.com/influxdata/influxdb-client-go/v2/api"
)

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
	p := influxdb2.NewPointWithMeasurement("esp32-sensor-data").
		AddTag("sensorID", sensorID).
		AddField("temperature", temp).
		AddField("humidity", humidity).
		SetTime(timestamp)

	return db.WriteAPI.WritePoint(context.Background(), p)
}
func (db *InfluxDB) QueryLatestSensorData() error {
	// TODO: implement latest data query
	return nil
}

func (db *InfluxDB) QueryTimeRangeSensorData() error {
	// TODO: implement time range data query
	return nil
}

func (db *InfluxDB) Close() {
	db.Client.Close()
}
