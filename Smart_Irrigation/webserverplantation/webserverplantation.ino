#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

#define DHTPIN 32
#define DHTTYPE DHT11

#define RELAY_PIN 33
#define SOIL_PIN 35

const char* ssid = "ROBODEMY";
const char* password = "robotics";

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

bool pumpState = false;

String getHTML() {

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp)) temp = 0;
  if (isnan(hum)) hum = 0;

  int moisture = 100 - (analogRead(SOIL_PIN) * 100 / 4095);
  moisture = constrain(moisture, 0, 100);

  String plantStatus;
  String statusColor;

  if (moisture < 60) {
    plantStatus = "⚠ Needs Watering";
    statusColor = "#ef4444";
  } else {
    plantStatus = "✓ Soil Moisture OK";
    statusColor = "#22c55e";
  }

  String relayStatus = pumpState ? "ON" : "OFF";

  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">

<title>Smart Plant Monitor</title>

<style>

*{
margin:0;
padding:0;
box-sizing:border-box;
font-family:Arial,sans-serif;
}

body{
background:#0f172a;
color:white;
padding:20px;
text-align:center;
}

h1{
margin-bottom:20px;
}

.container{
max-width:500px;
margin:auto;
}

.card{
background:#1e293b;
padding:20px;
margin:15px 0;
border-radius:18px;
box-shadow:0 5px 15px rgba(0,0,0,0.3);
}

.value{
font-size:36px;
font-weight:bold;
margin-top:10px;
}

.status{
font-size:24px;
font-weight:bold;
color:STATUSCOLOR;
}

button{
width:150px;
padding:15px;
border:none;
border-radius:12px;
font-size:18px;
cursor:pointer;
margin:8px;
font-weight:bold;
}

.on{
background:#22c55e;
color:white;
}

.off{
background:#ef4444;
color:white;
}

.pump{
font-size:22px;
font-weight:bold;
margin-top:10px;
}

.footer{
margin-top:20px;
font-size:14px;
color:#94a3b8;
}

</style>

<meta http-equiv="refresh" content="5">

</head>
<body>

<div class="container">

<h1>🌱 Smart Plant Dashboard</h1>

<div class="card">
<h2>🌡 Temperature</h2>
<div class="value">TEMP °C</div>
</div>

<div class="card">
<h2>💧 Humidity</h2>
<div class="value">HUM %</div>
</div>

<div class="card">
<h2>🌿 Soil Moisture</h2>
<div class="value">MOIST %</div>
</div>

<div class="card">
<h2>🚰 Watering Status</h2>
<div class="status">PLANTSTATUS</div>
<br>
<p>Watering Threshold: 60%</p>
</div>

<div class="card">
<h2>🔌 Pump Control</h2>

<a href="/on">
<button class="on">Pump ON</button>
</a>

<a href="/off">
<button class="off">Pump OFF</button>
</a>

<div class="pump">
Pump Status: RELAYSTATUS
</div>

</div>

<div class="footer">
ESP32 Smart Irrigation Monitor
</div>

</div>

</body>
</html>
)rawliteral";

  html.replace("TEMP", String(temp, 1));
  html.replace("HUM", String(hum, 1));
  html.replace("MOIST", String(moisture));
  html.replace("PLANTSTATUS", plantStatus);
  html.replace("STATUSCOLOR", statusColor);
  html.replace("RELAYSTATUS", relayStatus);

  return html;
}

void handleRoot() {
  server.send(200, "text/html", getHTML());
}

void handleOn() {
  digitalWrite(RELAY_PIN, HIGH);
  pumpState = true;

  server.sendHeader("Location", "/");
  server.send(303);
}

void handleOff() {
  digitalWrite(RELAY_PIN, LOW);
  pumpState = false;

  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {

  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  pinMode(SOIL_PIN, INPUT);

  dht.begin();

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");
  Serial.print("Open browser: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);

  server.begin();

  Serial.println("Web Server Started");
}

void loop() {
  server.handleClient();
}