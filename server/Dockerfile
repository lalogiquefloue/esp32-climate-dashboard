# Start from the official Golang image
FROM golang:1.25-alpine AS builder

WORKDIR /app

# Copy go mod and sum files
COPY go.mod go.sum ./
RUN go mod download

# Copy the rest of the source code
COPY . .

# Build the Go app
RUN go build -o server ./cmd/server

# Use a minimal image for running
FROM alpine:latest

WORKDIR /app

# Copy binary and configs
COPY --from=builder /app/server .
COPY configs ./configs

# Expose the port (match your app's port)
EXPOSE 8765

# Run the server
CMD ["./server"]