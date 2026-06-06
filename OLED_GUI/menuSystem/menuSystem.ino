#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Rotary Encoder
#define ENCODER_CLK 25
#define ENCODER_DT 26
#define ENCODER_SW 27

// Outputs
#define RED_LED 14
#define GREEN_LED 12
#define BLUE_LED 13
#define RELAY_PIN 33
#define BUZZER_PIN 4

bool status[6] = { 0 };  

int lastStateCLK, pulseCount = 0;

void setup() {
  Serial.begin(115200);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(ENCODER_CLK, INPUT_PULLUP);
  pinMode(ENCODER_DT, INPUT_PULLUP);
  pinMode(ENCODER_SW, INPUT_PULLUP);
  lastStateCLK = digitalRead(ENCODER_CLK);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(1);
  display.setTextSize(2);
  text("Press", 25, 10);
  text("Button", 15, 35);
  display.display();
}

void loop() {
  if (push(ENCODER_SW)) menu();
}