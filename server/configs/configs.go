package configs

import (
	"fmt"
	"os"

	"github.com/joho/godotenv"
)

type Config struct {
	Server   Server
	Database database
}

type Server struct {
	Address string
	Port    string
}

type database struct {
	DB string
}

func LoadConfigs() *Config {
	err := godotenv.Load("configs/.env")

	if err != nil {
		panic("Error loading .env file...")
	}

	c := &Config{
		Server: Server{
			Address: GetEnvValue("SERVER_ADDRESS"),
			Port:    GetEnvValue("SERVER_PORT"),
		},
		Database: database{
			DB: GetEnvValue("DATABASE_URL"),
		},
	}

	return c
}

func GetEnvValue(key string) string {
	value := os.Getenv(key)
	if value == "" {
		panic(fmt.Sprintf("Environment variable \"%s\" is not defined...", key))
	}
	return value
}
