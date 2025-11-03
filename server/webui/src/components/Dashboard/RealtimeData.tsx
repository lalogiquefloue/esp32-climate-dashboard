import { useEffect, useState } from "react"
import { getApiBaseUrl } from "../../utils/api"

type RealTimeData = {
  sensorID: string
  time: number
  temperature: number
  humidity: number
  status: string
}

const RealtimeData = () => {
  const [data, setData] = useState<RealTimeData | null>(null)
  const [error, setError] = useState<string | null>(null)

  useEffect(() => {
    const fetchData = () => {
      const url = getApiBaseUrl() + "/api/sensor/latest"
      fetch(url)
        .then(response => {
          if (!response.ok) throw new Error("Server error...")
          return response.json()
        })
        .then(json => setData(json.data))
        .catch(err => setError(err.message))
    }

    fetchData()
    const interval = setInterval(fetchData, 5000)

    return () => {
      // cleanup code
      clearInterval(interval)
    }
  }, [])

  if (error) return <p style={{ color: "red" }}>Error: {error}</p>
  if (!data) return <p>Loading...</p>

  return (
    <>
      <div className="grid grid-cols-2 gap-4 max-w-md p-2 mx-2 my-4 border bg-gray-50">
        <div className="flex flex-col">
          <span className="text-gray-500 text-sm">Sensor ID</span>
          <span className="font-semibold">{data.sensorID}</span>
        </div>
        <div className="flex flex-col">
          <span className="text-gray-500 text-sm">Time</span>
          <span className="font-semibold">{new Date(data.time * 1000).toLocaleString()}</span>
        </div>
        <div className="flex flex-col">
          <span className="text-gray-500 text-sm">Temperature</span>
          <span className="font-semibold">{data.temperature}°C</span>
          {/* TODO: Add daily mean */}
        </div>
        <div className="flex flex-col">
          <span className="text-gray-500 text-sm">Humidity</span>
          <span className="font-semibold">{data.humidity}%</span>
          {/* TODO: Add daily mean */}
        </div>
      </div>

    </>
  )
}

export default RealtimeData
