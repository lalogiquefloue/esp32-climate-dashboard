package handlers

import (
	"fmt"
	"net/http"
	"server/internal/configs"

	"github.com/gin-gonic/gin"
)

type Handlers struct {
	config           *configs.Config
	latestSensorData map[string]interface{}
}

func NewHandlers(config *configs.Config) *Handlers {
	return &Handlers{config: config, latestSensorData: nil}
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

func (h *Handlers) SensorData(c *gin.Context) {
	var jsonData map[string]interface{}

	if err := c.BindJSON(&jsonData); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "Invalid JSON"})
		return
	}

	fmt.Printf("Received sensor data: %+v\n", jsonData)
	// Store latest sensor data in memory
	h.latestSensorData = jsonData

	c.JSON(http.StatusOK, gin.H{
		"status": "success",
		"data":   jsonData,
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
