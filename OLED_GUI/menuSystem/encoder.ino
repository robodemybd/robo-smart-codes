int readEncoder() {
  int currentStateCLK = digitalRead(ENCODER_CLK);
  int direction = 0;
  if (currentStateCLK != lastStateCLK) {
    (digitalRead(ENCODER_DT) != currentStateCLK) ? direction = -1 : direction = 1;
    pulseCount += direction;
      delay(10);  // small debounce
  }
  lastStateCLK = currentStateCLK;
  if (abs(pulseCount) >= 2) {
    pulseCount = 0;
    return direction;
  } else return 0;
}

byte push(byte pin) {
  bool long_press = false;
  int t = 0;
  if (!digitalRead(pin)) {
    delay(20);
    while (!digitalRead(pin)) {
      delay(1);
      t++;
      if (t > 500 && !long_press) {
        long_press = true;
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        text("EXIT", 35, 24);
        display.display();
      }
    }
    if (long_press) return 2;
    return 1;
  }
  return 0;
}