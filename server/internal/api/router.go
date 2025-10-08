package api

import (
	"server/internal/api/handlers"
	"server/internal/configs"

	"github.com/gin-gonic/gin"
)

func RegisterRoutes(router *gin.Engine, config *configs.Config) {
	h := handlers.NewHandlers(config)

	router.GET("/", h.Home)
	router.POST("/sensor/data", h.SensorData)
	router.GET("/sensor/latest", h.GetLatestSensorData)
}
