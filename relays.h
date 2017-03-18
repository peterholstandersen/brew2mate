// ==== Relays =================================================================================

// Index  Chassis  Use   Relay  Wire   Arduino
// 0      -        -
// 1      Bottom   Heat  IN4    Gray   A1
// 2      Top      Cool  IN3    White  A2
// 3      12V      Fan   IN2    Black  A3
//
// Relay IN1 is not connected

int relay_pin[4] = { -1, A1, A2, A3 };

void setup_relays() {
  for (uint8_t i = 0; i < 4; i++)
    if (relay_pin[i] != -1)
      pinMode(relay_pin[i], OUTPUT);
}

// new_state: HIGH or LOW
void set_relay(int index, int new_state) {

  int pin = relay_pin[index];
  int current_state = digitalRead(pin);

  if (current_state == new_state)
    return;

  digitalWrite(pin, new_state);
}
