import * as d3 from "d3"
import { useEffect, useRef, useState } from "react"

interface DataChartProps {
  range?: string;
  sensorID?: string;
  field?: "temperature" | "humidity";
}

interface RawSensorDataItem {
  time: number; // Unix timestamp
  temperature: number;
  humidity: number;
}

interface RawSensorResponse {
  sensorID: string;
  range: string;
  count: number;
  data: RawSensorDataItem[];
}

interface SensorDataItem {
  time: Date;
  temperature: number;
  humidity: number;
}

interface SensorResponse {
  sensorID: string;
  range: string;
  count: number;
  data: SensorDataItem[];
}

const width = 800;
const height = 600;
const marginTop = 20;
const marginRight = 30;
const marginBottom = 75;
const marginLeft = 40;

const DataChart = ({ range, sensorID, field = "temperature" }: DataChartProps) => {
  // mostly adapted from https://observablehq.com/@d3/line-chart/2

  const [data, setData] = useState<SensorResponse | null>(null);
  const svgRef = useRef<SVGSVGElement | null>(null);

  const fetchData = async (range?: string, sensorID?: string) => {
    let url = "http://localhost:8765/api/sensor/range?"; // for local development
    // let url = "/api/sensor/range?"; 
    if (sensorID) url += `sensorID=${encodeURIComponent(sensorID)}&`;
    if (range) url += `range=${encodeURIComponent(range)}&`;

    const response = await fetch(url);
    const json = (await response.json()) as RawSensorResponse;
    const convertedData: SensorResponse = {
      ...json,
      data: json.data.map(item => ({
        ...item,
        time: new Date(item.time * 1000), // convert from Unix timestamp to JS Date
      })),
    };

    setData(convertedData);
  }

  // Fetch data peridically and when sensorID / range changes
  useEffect(() => {
    fetchData(range, sensorID);
    const interval = setInterval(() => fetchData(range, sensorID), 5000);
    return () => clearInterval(interval)
  }, [range, sensorID]);

  // Initialize an empty chart once
  useEffect(() => {
    const svg = d3
      .select(svgRef.current)
      .attr("width", width)
      .attr("height", height)
      .attr("viewBox", [0, 0, width, height])
      .attr("style", "max-width: 100%; height: auto;");

    // Clear any previous chart content
    svg.selectAll("*").remove();

    // Create scales with default domains
    const x = d3.scaleUtc().domain([new Date(), new Date()]).range([marginLeft, width - marginRight]);
    const y = d3.scaleLinear().domain([-100, 100]).range([height - marginBottom, marginTop]);

    // Append axes
    svg.append("g")
      .attr("class", "x-axis")
      .attr("transform", `translate(0,${height - marginBottom})`)
      .call(d3.axisBottom(x));

    svg.append("g")
      .attr("class", "y-axis")
      .attr("transform", `translate(${marginLeft},0)`)
      .call(d3.axisLeft(y));

    // Empty path for data
    svg.append("path")
      .attr("class", "data-line")
      .attr("fill", "none")
      .attr("stroke", "steelblue")
      .attr("stroke-width", 1.5);
  }, []);


  useEffect(() => {
    const yPadding = 0.75;

    if (!data || data.data.length === 0) return;
    const dataset = data.data;
    const svg = d3.select(svgRef.current);

    // Update X axis
    const xExtent = d3.extent(dataset, d => d.time); // TODO: check to base on range prop
    const x = d3.scaleUtc()
      .domain([xExtent[0] ?? new Date(), xExtent[1] ?? new Date()])
      .range([marginLeft, width - marginRight]);

    let tickFormat;
    let tickInterval;
    const diff = (xExtent[1]?.getTime() ?? 0) - (xExtent[0]?.getTime() ?? 0);

    if (diff < 1000 * 60 * 30) { // < 30 minutes
      tickFormat = d3.timeFormat("%H:%M");
      tickInterval = d3.timeMinute.every(1);
    }
    else if (diff < 1000 * 60 * 60) { // < 1 hour
      tickFormat = d3.timeFormat("%H:%M");
      tickInterval = d3.timeMinute.every(15);
    }
    else if (diff < 1000 * 60 * 60 * 12) { // < 12 hours
      tickFormat = d3.timeFormat("%H:%M");
      tickInterval = d3.timeMinute.every(15);
    }
    else if (diff < 1000 * 60 * 60 * 24) { // < 1 day
      tickFormat = d3.timeFormat("%H:%M");
      tickInterval = d3.timeHour.every(1);
    }
    else if (diff < 1000 * 60 * 60 * 24 * 7) { // < 1 week
      tickFormat = d3.timeFormat("%b %d" + " %H:%M");
      tickInterval = d3.timeHour.every(6);
    }
    else { // >= 1 week
      tickFormat = d3.timeFormat("%b %d, %Y");
      tickInterval = d3.timeMonth.every(1);
    }

    svg.select<SVGGElement>(".x-axis")
      .transition()
      .duration(500)
      .call(
        d3.axisBottom(x)
          .ticks(tickInterval)
          .tickFormat((d) => tickFormat(d as Date))
          .tickSizeOuter(0)
      );

    svg.selectAll(".x-axis text")
      .attr("transform", "rotate(-45)")
      .style("text-anchor", "end");

    // Update Y axis scale
    const yExtent = d3.extent(dataset, d => d[field]);
    const y = d3.scaleLinear()
      .domain([(yExtent[0] ?? -100) - yPadding, (yExtent[1] ?? 100) + yPadding])
      .range([height - marginBottom, marginTop]);

    // Y axis
    svg.select<SVGGElement>(".y-axis")
      .transition().duration(500)
      .call(d3.axisLeft(y));

    // horizontal grid lines
    svg.selectAll(".y-grid").remove(); // remove previous grid lines
    svg.append("g")
      .attr("class", "y-grid")
      .attr("transform", `translate(${marginLeft},0)`)
      .call(
        d3.axisLeft(y)
          .tickSize(-(width - marginLeft - marginRight))
          .tickFormat(() => "")
      )
      .selectAll("line")
      .attr("stroke-dasharray", "4,4")
      .attr("stroke-opacity", 0.2);

    svg.selectAll(".y-grid path").remove(); // remove solid axis line

    // Line generator
    const line = d3.line<SensorDataItem>()
      .x(d => x(d.time))
      .y(d => y(d[field]));

    // Update line
    svg.select<SVGPathElement>(".data-line")
      .datum(dataset)
      .transition().duration(500)
      .attr("d", line)
      .attr("stroke", field === "temperature" ? "red" : "blue")
      .attr("stroke-width", 1);

    // INTERACTIVITY
    const timeFormatter = d3.timeFormat("%Y-%m-%d %H:%M:%S");

    // Remove any previous tooltip layers
    svg.selectAll(".tooltip-elements").remove();
    d3.selectAll(`.chart-tooltip-${field}`).remove();

    // Tooltip group
    const tooltipGroup = svg.append("g")
      .attr("class", "tooltip-elements")
      .style("display", "none");

    tooltipGroup.append("circle")
      .attr("r", 3)
      .attr("fill", "black");

    // HTML tooltip
    const tooltipDiv = d3.select("body")
      .append("div")
      .attr("class", `chart-tooltip-${field}`)
      .style("position", "absolute")
      .style("background", "rgba(255,255,255,0.9)")
      .style("padding", "6px 8px")
      .style("border", "1px solid #aaa")
      .style("border-radius", "6px")
      .style("pointer-events", "none")
      .style("font-size", "12px")
      .style("display", "none");

    // Overlay rect for mouse tracking
    svg.append("rect")
      .attr("class", "tooltip-elements")
      .attr("fill", "none")
      .attr("pointer-events", "all")
      .attr("width", width - marginLeft - marginRight)
      .attr("height", height - marginTop - marginBottom)
      .attr("x", marginLeft)
      .attr("y", marginTop)
      .on("mouseover", () => {
        tooltipGroup.style("display", null);
        tooltipDiv.style("display", null);
      })
      .on("mouseout mouseleave", () => {
        tooltipGroup.style("display", "none");
        tooltipDiv.style("display", "none");
      })
      .on("mousemove", (event) => {

        const [xm] = d3.pointer(event);
        const xDate = x.invert(xm);

        // Find nearest data point
        const i = d3.bisector((d: SensorDataItem) => d.time).center(dataset, xDate);
        const d = dataset[i];
        if (!d) return;

        tooltipGroup.attr("transform", `translate(${x(d.time)},${y(d[field])})`);

        const unit = field === "temperature" ? "°C" : "%";
        tooltipDiv
          .style("left", `${event.pageX + 12}px`)
          .style("top", `${event.pageY - 20}px`)
          .html(`
              <strong>${field}:</strong> ${d[field].toFixed(2)} ${unit}<br/>
              <small>${timeFormatter(new Date(d.time))}</small>
            `);
      });
  }, [data, field]);


  return (
    <div>
      <svg ref={svgRef}></svg>
    </div>
  )
}

export default DataChart
