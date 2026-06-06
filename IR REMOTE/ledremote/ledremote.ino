#define DECODE_NEC
#include <IRremote.hpp>

#define IR_RECEIVE_PIN 15

#define RED_PIN   14
#define GREEN_PIN 12
#define BLUE_PIN  13

bool redState = false;
bool greenState = false;
bool blueState = false;

unsigned long lastPress = 0;
const unsigned long debounceTime = 300; // ms

void updateLED() {
  digitalWrite(RED_PIN, redState);
  digitalWrite(GREEN_PIN, greenState);
  digitalWrite(BLUE_PIN, blueState);
}

void setup() {
  Serial.begin(115200);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  updateLED();

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("IR RGB Toggle Controller Ready");
}

void loop() {

  if (IrReceiver.decode()) {

    // Ignore NEC repeat frames
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
      IrReceiver.resume();
      return;
    }

    // Software debounce
    if (millis() - lastPress < debounceTime) {
      IrReceiver.resume();
      return;
    }

    lastPress = millis();

    uint8_t code = IrReceiver.decodedIRData.command;

    Serial.print("Received: 0x");
    Serial.println(code, HEX);

    switch (code) {

      case 0x4:   // Toggle RED
        redState = !redState;
        Serial.print("RED: ");
        Serial.println(redState ? "ON" : "OFF");
        break;

      case 0x5:   // Toggle GREEN
        greenState = !greenState;
        Serial.print("GREEN: ");
        Serial.println(greenState ? "ON" : "OFF");
        break;

      case 0x6:   // Toggle BLUE
        blueState = !blueState;
        Serial.print("BLUE: ");
        Serial.println(blueState ? "ON" : "OFF");
        break;

      default:
        break;
    }

    updateLED();

    Serial.print("Current State -> R:");
    Serial.print(redState);
    Serial.print(" G:");
    Serial.print(greenState);
    Serial.print(" B:");
    Serial.println(blueState);

    IrReceiver.resume();
  }
}