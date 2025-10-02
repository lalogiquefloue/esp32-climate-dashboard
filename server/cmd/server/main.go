package main

import (
	"fmt"
	"net/http"
	"server/configs"

	"github.com/gin-gonic/gin"
)

var config *configs.Config

func main() {
	// load configs
	config = configs.LoadConfigs()

	// init Gin router
	router := gin.Default()

	// register routes
	router.GET("/", homePage)
	router.POST("/sensor/data", sensorDataHandler)

	// start server
	addr := config.Server.Address + ":" + config.Server.Port
	router.Run(addr)
}

func homePage(c *gin.Context) {
	c.JSON(http.StatusOK, gin.H{
		"message":     "esp32-climate-dashboard server",
		"server-url":  config.Server.Address,
		"server-port": config.Server.Port,
	})
}

func sensorDataHandler(c *gin.Context) {
	var jsonData map[string]interface{}

	// Bind JSON body to map
	if err := c.BindJSON(&jsonData); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "Invalid JSON"})
		return
	}

	// Print received data to server console
	fmt.Printf("Received sensor data: %+v\n", jsonData)

	// Respond back to ESP32
	c.JSON(http.StatusOK, gin.H{
		"status": "success",
		"data":   jsonData,
	})
}
