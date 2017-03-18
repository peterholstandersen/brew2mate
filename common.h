// Forward

float get_actual_temperature();


// Common functions

void print_boolean(const char *prefix, boolean value) {
  Serial.print(prefix);
  Serial.print("=");
  Serial.print(value);
  Serial.print("  ");
}

void print_long(const char *prefix, long value) {
  Serial.print(prefix);
  Serial.print("=");
  Serial.print(value);
  Serial.print("  ");
}

void print_float(const char *prefix, float value) {
  Serial.print(prefix);
  Serial.print("=");
  Serial.print(value);
  Serial.print("  ");
}

void print_mode(const char *prefix, int mode) {
  Serial.print(prefix);
  Serial.print("=");
  switch (mode) {
  case IDLE:    Serial.print("IDLE   "); break;
  case HEATING: Serial.print("HEATING"); break;
  case COOLING: Serial.print("COOLING"); break;
  default:      Serial.print("ILLEGAL"); break;
  }
  Serial.print("  ");
}

