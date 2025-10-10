package configs

import (
	"fmt"
	"os"

	"github.com/joho/godotenv"
)

type Config struct {
	Server   Server
	Database Database
}

type Server struct {
	Address string
	Port    string
}

type Database struct {
	Bucket string
	Org    string
	Token  string
	URL    string
}

func LoadConfigs() *Config {
	// Load .env only if not in Docker for local development
	if os.Getenv("DOCKER_ENV") != "true" {
		err := godotenv.Load("../../.env")
		if err != nil {
			panic("Error loading .env file...")
		}
	}

	c := &Config{
		Server: Server{
			Address: GetEnvValue("SERVER_ADDRESS"),
			Port:    GetEnvValue("SERVER_PORT"),
		},
		Database: Database{
			Bucket: GetEnvValue("INFLUXDB_BUCKET"),
			Org:    GetEnvValue("INFLUXDB_ORG"),
			Token:  GetEnvValue("INFLUXDB_TOKEN"),
			URL:    GetEnvValue("INFLUXDB_URL"),
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
