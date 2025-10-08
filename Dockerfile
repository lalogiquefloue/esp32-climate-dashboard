# Stage 1: Build Go app
FROM golang:1.25-alpine AS gobuilder

WORKDIR /app

# Copy Go mod/sum files first for dependency caching
COPY server/go.mod server/go.sum ./
RUN go mod download

# Copy the Go source
COPY server/ ./

# Build the Go binary
RUN go build -o server ./cmd/server

# -------------------------------------------
# Stage 2: Build the React frontend
FROM node:20-alpine AS uibuilder

WORKDIR /app

# Copy only webui files
COPY server/webui/package*.json ./
RUN npm ci --silent

COPY server/webui ./
RUN npm run build

# -------------------------------------------
# Stage 3: Minimal runtime image
FROM alpine:latest

WORKDIR /app

# Add HTTPS root certs (for external API calls)
RUN apk --no-cache add ca-certificates

# Copy Go binary
COPY --from=gobuilder /app/server .
# Copy configs
COPY server/internal/configs ./internal/configs
# Copy built frontend
COPY --from=uibuilder /app/dist ./webui/dist

# Expose app port
EXPOSE 8765

# Run the Go server
CMD ["./server"]
