let lastUpdated;
let isLoading = false;
async function fetchWeatherData() {
  if (isLoading) {
    return;
  }
  try {
    // Show the spinner
    isLoading = true;
    document.getElementById("spinner").style.display = "block";

    const response = await fetch("http://localhost:3000/api/weather");
    const data = await response.json();

    document.getElementById("temperature").innerText = `${data.temperature} °C`;
    document.getElementById("humidity").innerText = `${data.humidity} %`;
    document.getElementById("wind-speed").innerText = `${data.windSpeed} km/h`;

    // Store the last updated time
    lastUpdated = new Date();
    isLoading = false;
  } catch (error) {
    console.error("Error fetching weather data:", error);
  } finally {
    // Hide the spinner
    document.getElementById("spinner").style.display = "none";
  }
}
function updateTimeSinceLastUpdate() {
  if (lastUpdated) {
    const now = new Date();
    const secondsSinceUpdate = Math.floor((now - lastUpdated) / 1000);
    const minutes = Math.floor(secondsSinceUpdate / 60);
    const seconds = secondsSinceUpdate % 60;

    document.getElementById(
      "last-updated"
    ).innerText = `Last updated: ${minutes}:${seconds
      .toString()
      .padStart(2, "0")}`;
  } else {
    document.getElementById("last-updated").innerText = "Last updated: N/A";
  }
}
// Add a click event listener to the Update Weather Data button
document
  .getElementById("update-btn")
  .addEventListener("click", fetchWeatherData);

setInterval(updateTimeSinceLastUpdate, 1000);
fetchWeatherData();
updateTimeSinceLastUpdate();
