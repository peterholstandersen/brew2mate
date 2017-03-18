// ==== Keys =========================================================================

#define KEYS_PIN		A0
#define DEBOUNCE_DELAY          50

uint8_t lastKeyPressed = 0;
int lastKeyReading = 0;
int stableKeyReading = 0;  // No key pressed
long lastDebounceTime = 0;
long key_pressed_time = -100000;

void loop_keys() {
  int unstableKeyReading;
  
  unstableKeyReading = (analogRead(KEYS_PIN) + 135/2) / 135;

  if (lastKeyReading != unstableKeyReading) {
    lastDebounceTime = millis();
  }
  lastKeyReading = unstableKeyReading;
  
  if (millis() - lastDebounceTime > DEBOUNCE_DELAY) {
    stableKeyReading = unstableKeyReading;
  }

  if (stableKeyReading != lastKeyPressed) {
    // Serial.print("Triggering key event, stableKeyReading: ");
    // Serial.println(stableKeyReading);

    switch (stableKeyReading) {
      case 1: key1_event(); break;
      case 2: key2_event(); break;
      case 3: key3_event(); break;
      case 4: key4_event(); break;
      case 5: key5_event(); break;
      case 6: key6_event(); break;
      default: break;
    }
    key_pressed_time = millis();
    lastKeyPressed = stableKeyReading;
  }
}
