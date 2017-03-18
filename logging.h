// ==== Logging ================================================================================

#define LOG(x)   Serial.print(x);

void log_status() {
  const long log_interval = 5000;
  static long next_log_time = -1;
  long now = millis();
  if (now < next_log_time) {
    return;
  }
  next_log_time = now + log_interval;

  LOG("[");
  LOG(now);
  LOG("] ");

  print_mode("current_mode", current_mode);
  print_mode("desired_mode", desired_mode);

  print_long("allowed_idle_time", allowed_idle_time);
  print_long("allowed_heat_time", allowed_heat_time);
  print_long("allowed_cool_time", allowed_cool_time);

  print_float("target_temperature", target_temperature);
  
  for (uint8_t i = 0; i < 4; i++) {
    Serial.print("  actual[");
    Serial.print(i);
    Serial.print("]");
    print_float("", actual_temperatures[i]);
  }

  LOG("\n");
}
