package handlers

import (
	"fmt"
	"net/http"
	"server/internal/configs"
	"server/internal/db"
	"server/internal/models"
	"time"

	"github.com/gin-gonic/gin"
)

type Handlers struct {
	config           *configs.Config
	db               *db.InfluxDB
	latestSensorData interface{}
}

func NewHandlers(config *configs.Config) *Handlers {
	return &Handlers{
		config:           config,
		db:               db.NewInfluxDB(config),
		latestSensorData: nil,
	}
}

func (h *Handlers) Home(c *gin.Context) {
	c.JSON(http.StatusOK, gin.H{
		"message":         "esp32-climate-dashboard server",
		"server-url":      h.config.Server.Address,
		"server-port":     h.config.Server.Port,
		"database-url":    h.config.Database.URL,
		"database-bucket": h.config.Database.Bucket,
		"database-org":    h.config.Database.Org,
		"latest-data":     h.latestSensorData,
		"status":          "running",
	})
}

// POST /sensor/data accepts JSON sensor data
func (h *Handlers) ReceiveSensorData(c *gin.Context) {

	var payload models.SensorData

	if err := c.BindJSON(&payload); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "Invalid JSON"})
		return
	}

	fmt.Printf("Received sensor data: %+v\n", payload)

	// Convert UNIX timestamp
	timestamp := time.Unix(payload.Time, 0)

	// Store latest sensor data in memory
	h.latestSensorData = payload

	// Ingest received data into InfluxDB
	if err := h.db.WriteSensorData(payload.SensorID, payload.Temperature, payload.Humidity, timestamp); err != nil {
		fmt.Println("InfluxDB write error:", err)
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"status": "success",
		"data":   payload,
	})
}

// GET /sensor/latest returns the latest sensor data
func (h *Handlers) GetLatestSensorData(c *gin.Context) {
	if h.latestSensorData == nil {
		c.JSON(http.StatusNotFound, gin.H{"error": "No sensor data available"})
		return
	}
	c.JSON(http.StatusOK, gin.H{
		"status": "success",
		"data":   h.latestSensorData,
	})
}

func (h *Handlers) GetRangeSensorData(c *gin.Context) {
	sensorID := c.Query("sensorID")
	duration := c.Query("range") // e.g. 15m, 1h, 1d

	if duration == "" {
		duration = "1h" // default to 1 hour
	}

	data, err := h.db.QueryRecentSensorData(sensorID, duration)
	if err != nil {
		c.JSON(http.StatusInternalServerError, gin.H{"error": err.Error()})
		return
	}

	c.JSON(http.StatusOK, gin.H{
		"sensorID": sensorID,
		"range":    duration,
		"count":    len(data),
		"data":     data,
	})
}
