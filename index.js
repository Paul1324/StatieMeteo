const express = require("express");
const axios = require("axios");

const app = express();
const PORT = process.env.PORT || 3000;

app.use(express.static("public"));

app.get("/api/weather", async (req, res) => {
  try {
    console.log("Fetching data from ESP8266");
    const esp8266Url = "http://192.168.137.139";
    const response = await axios.get(esp8266Url);
    console.log(response);
    res.status(200).send(response.data);
  } catch (error) {
    res
      .status(500)
      .send({ message: "Error fetching data from ESP8266", error });
  }
});

app.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`);
});
