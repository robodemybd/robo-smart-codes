void text(String s, int x, int y) {
  display.setCursor(x, y);
  display.print(s);
}

String menu_options(byte a) {
  if (a == 1) return "Red LED    ";
  if (a == 2) return "Green LED  ";
  if (a == 3) return "Blue LED   ";
  if (a == 4) return "Relay      ";
  if (a == 5) return "Buzzer     ";
  return "";
}

void menu() {
  byte index = 1;
  byte oldIndex = 0;
  const byte limit = 5;
  display.setTextSize(1);
  while (1) {
    if (oldIndex != index) {
      oldIndex = index;
      display.clearDisplay();
      for (byte i = 1; i <= limit; i++) {
        String value = status[i] ? "ON" : "OFF";
        (i == index) ? display.setTextColor(SSD1306_BLACK, SSD1306_WHITE) : display.setTextColor(SSD1306_WHITE);
        text(menu_options(i) + value, 0, (i - 1) * 12);
      }
      display.display();
    }
    int e = readEncoder();
    if (e == 1) {
      index++;
      if (index > limit) index = 1;
      oldIndex = 0;
    }
    if (e == -1) {
      if (index == 1) index = limit;
      else index--;
      oldIndex = 0;
    }

    byte r = push(ENCODER_SW);
    if (r == 1) {
      status[index] = !status[index];
      appliance_update(index);
      oldIndex = 0;
    }
    if (r == 2)break;
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(1);
  text("ROBODEMY", 10, 12);
  text("MENU", 32, 40);
  display.display();
}