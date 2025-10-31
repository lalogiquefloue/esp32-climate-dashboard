import { useState } from "react";
import RealtimeData from "./Dashboard/RealtimeData"
import DataChart from "./Dashboard/DataChart"

const Dashboard = () => {
  const [range, setRange] = useState("3h");
  // const [field, setField] = useState<"temperature" | "humidity">("temperature");
  // const [sensorID, setSensorID] = useState("");

  const ranges = ["5m", "15m", "30m", "1h", "3h", "6h", "12h", "24h", "7d", "30d", "365d"];
  // const fields = ["temperature", "humidity"];
  // const sensorOptions = [] // TODO: implement sensorID selection, fetch available sensors

  return (
    <div>
      <div>
        <h1 className="text-2xl bold ml-2 mb-4 underline">ESP32 Climate Dashboard</h1>
        <h1 className="text-xl bold ml-2">Live Data</h1>
        <RealtimeData />
      </div>
      <br />
      <div>
        <h1 className="text-xl bold ml-2 mb-4">Historical Data</h1>
        <div className="flex items-center gap-3 ml-2">
          <label htmlFor="range" className="text-gray-700 font-medium">
            Select Range:
          </label>

          <select
            id="range"
            value={range}
            onChange={(e) => setRange(e.target.value)}
            className="block w-40 rounded-lg border border-gray-300 bg-white px-3 py-1 text-sm text-gray-700 shadow-sm"
          >
            {ranges.map((r) => (
              <option key={r} value={r}>
                {r}
              </option>
            ))}
          </select>
        </div>

        <DataChart range={range} field="temperature" />
        <DataChart range={range} field="humidity" />
      </div>
    </div>
  )
}

export default Dashboard