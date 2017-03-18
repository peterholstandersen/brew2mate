// ==== Logic ==================================================================================

#define HEAT_INDEX  1
#define COOL_INDEX  2
#define FAN_INDEX   3

long allowed_idle_time = -1;
long allowed_heat_time = -1;
long allowed_cool_time = -1;


// Keys on the front of the box are numbered from right to left

void key1_event() {
  toggle_screensaver();
}

void key2_event() {
  // Allow changing mode now
  allowed_idle_time = -1;
  allowed_heat_time = -1;
  allowed_cool_time = -1;
}

// For key3-key6:
// If the screensaver is active, then turn if off and consume the keypress.
// Otherwise, do the action and keep the screensaver off.

void key3_event() {
  if (is_screensaver_active()) {
    screensaver_off();
    return;
  }
  // target_temperature = 76.0;
  screensaver_off();
}

void key4_event() {
  if (is_screensaver_active()) {
    screensaver_off();
    return;
  }
  // target_temperature = 21.0;
  screensaver_off();
}

void key5_event() {
  if (is_screensaver_active()) {
    screensaver_off();
    return;
  }
  // target_temperature = target_temperature + 0.1;
  // target_temperature = round(target_temperature * 10.0) / 10.0;
  screensaver_off();
}

void key6_event() {
  if (is_screensaver_active()) {
    screensaver_off();
    return;
  }
  // target_temperature = target_temperature - 0.1;
  // target_temperature = round(target_temperature * 10.0) / 10.0;
  screensaver_off();
}

void set_mode(int desired_mode) {

  long now = millis();

  current_mode = desired_mode;
  
  int heat = LOW;
  int cool = LOW;

  switch (current_mode) {
    case IDLE:
      heat = LOW;
      cool = LOW;
      break;

    case HEATING:
      heat = HIGH;
      cool = LOW;
      allowed_heat_time = now + START_HEATING_DELAY;
      allowed_cool_time = now + MIN_HEATING_TIME;
      allowed_idle_time = now + MIN_HEATING_TIME;
      break;

    case COOLING:
      heat = LOW;
      cool = HIGH;
      allowed_cool_time = now + START_COOLING_DELAY;
      allowed_heat_time = now + MIN_COOLING_TIME;
      allowed_idle_time = now + MIN_COOLING_TIME;
      break;
  }

  set_relay(HEAT_INDEX, heat);
  set_relay(COOL_INDEX, cool);
}

int check_desired_mode() {

  float actual_temperature = get_actual_temperature();
  float temp_diff = actual_temperature - target_temperature;
  if (temp_diff < 0.0)
    temp_diff = -temp_diff;

  if (temp_diff < TEMPERATURE_TOLERANCE) {
    desired_mode = IDLE;
  }
  else {
    if (actual_temperature > target_temperature)
      desired_mode = COOLING;
    else
      desired_mode = HEATING;
   }
}

int check_idle(float actual_temperature) {
  if (actual_temperature > target_temperature + TEMPERATURE_TOLERANCE)
      return COOLING;
  if (actual_temperature < target_temperature - TEMPERATURE_TOLERANCE)
      return HEATING;
  return IDLE;
}

int check_heating(float actual_temperature) {
  if (actual_temperature > target_temperature + TEMPERATURE_TOLERANCE)
    return COOLING;
  if (actual_temperature >= target_temperature) 
    return IDLE;
  return HEATING;
}

int check_cooling(float actual_temperature) {
  if (actual_temperature < target_temperature - TEMPERATURE_TOLERANCE)
    return HEATING;
  if (actual_temperature <= target_temperature)
    return IDLE;
  return COOLING;
}

void loop_logic() {
  float actual_temperature = get_actual_temperature();
  
  // Record min/max temperatures
  if (actual_temperature < min_temperature)
    min_temperature = actual_temperature;
  if (actual_temperature > max_temperature)
    max_temperature = actual_temperature;

  // Determine desired mode
  switch (current_mode) {
  case IDLE:    desired_mode = check_idle(actual_temperature);    break;
  case HEATING: desired_mode = check_heating(actual_temperature); break;
  case COOLING: desired_mode = check_cooling(actual_temperature); break;
  }

  long now = millis();

  // Set desired mode, if it is allowed
  switch (desired_mode) {
  case IDLE:    if (now > allowed_idle_time) set_mode(IDLE); break;
  case HEATING: if (now > allowed_heat_time) set_mode(HEATING); break;
  case COOLING: if (now > allowed_cool_time) set_mode(COOLING); break;
  }
}
