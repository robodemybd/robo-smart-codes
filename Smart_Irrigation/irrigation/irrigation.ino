#include <DHT.h>

#define DHTPIN 32
#define DHTTYPE DHT11

#define SOIL_PIN 35
#define RELAY_PIN 33

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);

  // Relay OFF at startup
  digitalWrite(RELAY_PIN, LOW);

  dht.begin();

  Serial.println("Smart Plant Watering System Started");
}

void loop() {

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  int moisture = 100 - (analogRead(SOIL_PIN) * 100 / 4095);
  moisture = constrain(moisture, 0, 100);

  bool pumpOn = false;

  if (moisture < 30) {
    digitalWrite(RELAY_PIN, HIGH);
    pumpOn = true;
  } else {
    digitalWrite(RELAY_PIN, LOW);
    pumpOn = false;
  }

  Serial.println("--------------------------------");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Soil Moisture: ");
  Serial.print(moisture);
  Serial.println(" %");

  Serial.print("Pump: ");
  Serial.println(pumpOn ? "ON" : "OFF");

  delay(2000);
}