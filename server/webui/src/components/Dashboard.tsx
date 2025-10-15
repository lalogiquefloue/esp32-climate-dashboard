import { useState } from "react";
import RealtimeDataTable from "./Dashboard/RealtimeDataTable"
import DataChart from "./Dashboard/DataChart"

const Dashboard = () => {
  const [range, setRange] = useState("1h");
  // const [sensorID, setSensorID] = useState("");

  const ranges = ["5m", "15m", "30m", "1h", "6h", "12h", "24h", "7d", "30d", "365d"];
  // const sensorOptions = [] // TODO: implement sensorID selection, fetch available sensors

  return (
    <div>
        <div>
          <h1>ESP32 Climate Dashboard</h1>
          <RealtimeDataTable />
        </div>

        <br />

        <div>
          <h2>Historical Data</h2>
          <label htmlFor="range">Select Range: </label>
          <select value={range} onChange={(e) => setRange(e.target.value)}>
            {ranges.map((r) => (<option key={r} value={r}>{r}</option>))}
          </select>

          <DataChart range={range} /> 
        </div>
    </div>
  )
}

export default Dashboard