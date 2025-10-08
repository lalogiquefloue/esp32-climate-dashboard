package server

import (
	"fmt"
	"server/internal/api"
	"server/internal/configs"

	"github.com/gin-contrib/cors"
	"github.com/gin-gonic/gin"
)

type Server struct {
	config *configs.Config
	router *gin.Engine
}

func New(config *configs.Config) *Server {
	router := gin.Default()
	router.Use(cors.Default())
	api.RegisterRoutes(router, config)

	return &Server{
		config: config,
		router: router,
	}
}

func (s *Server) Run() {
	addr := fmt.Sprintf("%s:%s", s.config.Server.Address, s.config.Server.Port)
	s.router.Run(addr)
}
