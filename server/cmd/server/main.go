package main

import (
	"server/internal/configs"
	"server/internal/server"
)

func main() {
	config := configs.LoadConfigs()
	server := server.New(config)
	server.Run()
}
