#define DECODE_NEC
#include <IRremote.hpp>

#define IR_RECEIVE_PIN 15

void setup() {
  Serial.begin(115200);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("IR Receiver Ready");
}

void loop() {


  if (IrReceiver.decode()) {

    if (IrReceiver.decodedIRData.protocol != UNKNOWN) {

      Serial.print("HEX: 0x");
      Serial.println(IrReceiver.decodedIRData.command, HEX);

      Serial.print("DEC: ");
      Serial.println(IrReceiver.decodedIRData.command);
      Serial.println();
    }

    IrReceiver.resume();
  }
}