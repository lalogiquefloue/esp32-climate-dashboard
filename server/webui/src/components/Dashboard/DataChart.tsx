// import * as d3 from "d3"
import { useEffect, useState } from "react"

interface SensorDataItem {
  time: number;
  temperature: number;
  humidity: number;
}

interface SensorResponse {
  sensorID: string;
  range: string;
  count: number;
  data: SensorDataItem[];
}

interface DataChartProps {
  range?: string; 
  sensorID?: string;
}

const DataChart = ({ range, sensorID }: DataChartProps) => {
  // based on https://observablehq.com/@d3/line-chart/2
  const [data, setData] = useState<SensorResponse | null>(null);

  const fetchData = async (range?: string, sensorID?: string) => {

      let url = "http://localhost:8765/api/sensor/range?"; // for local development
      // let url = "/api/sensor/range?"; 
      if (sensorID) url += `sensorID=${encodeURIComponent(sensorID)}&`;
      if (range) url += `range=${encodeURIComponent(range)}&`;

      const response = await fetch(url); 
      const json = await response.json();

      // Convert time from Unix seconds to JS Date
      const convertedData = {
        ...json,
        data: json.data.map((item: SensorDataItem) => ({
          ...item,
          time: new Date(item.time * 1000),
        })),
      };

      setData(convertedData);
    }
      
  useEffect(() => {
        fetchData(range, sensorID);
        const interval = setInterval(() => fetchData(range, sensorID), 5000);
        return () => clearInterval(interval)
  }, [range, sensorID]);

    return (
    <div>
        {data ? <pre>{JSON.stringify(data, null, 2)}</pre> : <p>Loading...</p>}
    </div>
    )
}

export default DataChart
