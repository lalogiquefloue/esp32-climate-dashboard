package api

import (
	"server/internal/api/handlers"
	"server/internal/configs"

	"github.com/gin-gonic/contrib/static"
	"github.com/gin-gonic/gin"
)

func RegisterRoutes(router *gin.Engine, config *configs.Config) {
	h := handlers.NewHandlers(config)

	// Serve frontend static files
	router.Use(static.Serve("/", static.LocalFile("./webui/dist", true)))

	// API Endpoints
	api := router.Group("/api")
	{
		api.GET("/status", h.Home)
		api.GET("/sensor/latest", h.GetLatestSensorData)
		api.POST("/sensor/data", h.ReceiveSensorData)
	}

}
