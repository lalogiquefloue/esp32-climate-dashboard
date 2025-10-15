import { useEffect, useState } from "react"
import './RealtimeDataTable.css'

type RealTimeData = {
  sensorID: string
  time: number
  temperature: number
  humidity: number
  status: string
}

const RealtimeDataTable = () => {
  const [data, setData] = useState<RealTimeData | null>(null)
  const [error, setError] = useState<string | null>(null)

  useEffect(() => {
    const fetchData = () => {
        fetch("http://localhost:8765/api/sensor/latest") // for local dev
        // fetch("/api/sensor/latest")
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
    <table className="data-table">
      <thead>
        <tr>
          <th>SensorID</th>
          <th>Last update</th>
          <th>Temperature (°C)</th>
          <th>Humidity (%)</th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td>{data.sensorID}</td>
          <td>{new Date(data.time * 1000).toLocaleString()}</td>
          <td>{data.temperature}</td>
          <td>{data.humidity}</td>
        </tr>
      </tbody>
    </table>
  )
}

export default RealtimeDataTable
