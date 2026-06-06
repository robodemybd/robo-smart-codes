void appliance_update(byte id) {
  switch (id) {
    case 1:
      digitalWrite(RED_LED, status[1]);
      break;
    case 2:
      digitalWrite(GREEN_LED, status[2]);
      break;
    case 3:
      digitalWrite(BLUE_LED, status[3]);
      break;
    case 4:
      digitalWrite(RELAY_PIN, status[4]);
      break;
    case 5:
      digitalWrite(BUZZER_PIN, status[5]);
      break;
  }
}