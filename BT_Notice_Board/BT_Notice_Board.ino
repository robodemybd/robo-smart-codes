#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "BluetoothSerial.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
BluetoothSerial SerialBT;

String notice = "Welcome to Robodemy";

void showNotice(String msg)
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  int y = 0;
  int start = 0;

  while (start < msg.length())
  {
    String line = msg.substring(start, start + 21);

    display.setCursor(0, y);
    display.println(line);

    start += 21;
    y += 10;

    if (y > 54) break;
  }

  display.display();
}

void setup()
{
  Serial.begin(115200);

  SerialBT.begin("Robodemy Notice Board");

  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    while (1);
  }

  display.clearDisplay();
  display.display();

  showNotice(notice);

  Serial.println("Bluetooth Notice Board Ready");
}

void loop()
{
  if (SerialBT.available())
  {
    notice = SerialBT.readStringUntil('\n');
    notice.trim();

    if (notice.length() > 0)
    {
      Serial.println("New Notice:");
      Serial.println(notice);

      showNotice(notice);
    }
  }
}