export function getApiBaseUrl(): string {
  const host = window.location.hostname;
  const port = window.location.port;

  // Running via Vite dev server (localhost:5173)
  if (host === "localhost" && port === "5173") {
    return "http://localhost:8765";
  }

  // Served by Go (in Docker or production)
  return "";
}
