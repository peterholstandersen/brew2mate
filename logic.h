// ==== Logic ==================================================================================

#define HEAT_INDEX  1
#define COOL_INDEX  2
#define FAN_INDEX   3

long allowed_idle_time = -1;
long allowed_heat_time = START_UP_DELAY;
long allowed_cool_time = START_UP_DELAY;


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
#if TEST_MODE
  target_temperature = target_temperature + 0.1;
  target_temperature = round(target_temperature * 10.0) / 10.0;
#endif
  screensaver_off();
}

void key6_event() {
  if (is_screensaver_active()) {
    screensaver_off();
    return;
  }
#if TEST_MODE
  target_temperature = target_temperature - 0.1;
  target_temperature = round(target_temperature * 10.0) / 10.0;
#endif
  screensaver_off();
}

void set_allowed_idle_time(long timestamp) {
  if (timestamp > allowed_idle_time)
    allowed_idle_time = timestamp;
}

void set_allowed_heat_time(long timestamp) {
  if (timestamp > allowed_heat_time)
    allowed_heat_time = timestamp;
}

void set_allowed_cool_time(long timestamp) {
  if (timestamp > allowed_cool_time)
    allowed_cool_time = timestamp;
}

void set_mode(int desired_mode) {

  long now = millis();

  int heat = LOW;
  int cool = LOW;

  current_mode = desired_mode;

  switch (current_mode) {
    case IDLE:
      heat = LOW;
      cool = LOW;
      set_allowed_cool_time(now + MINIMUM_IDLE_TIME);
      set_allowed_heat_time(now + MINIMUM_IDLE_TIME);
      break;

    case HEATING:
      heat = HIGH;
      cool = LOW;
      set_allowed_idle_time(now + MINIMUM_HEAT_TIME);
      set_allowed_cool_time(now + COOL_HEAT_DELAY);
      break;

    case COOLING:
      heat = LOW;
      cool = HIGH;
      set_allowed_idle_time(now + MINIMUM_COOL_TIME);
      set_allowed_heat_time(now + COOL_HEAT_DELAY);
      break;
  }

  set_relay(HEAT_INDEX, heat);
  set_relay(COOL_INDEX, cool);
}

// When in idle determine which mode we want to change to
int check_idle(float actual_temperature) {
  if (actual_temperature > target_temperature + TEMPERATURE_TOLERANCE)
      return COOLING;
  if (actual_temperature < target_temperature - TEMPERATURE_TOLERANCE)
      return HEATING;
  return IDLE;
}

// When in heating mode determinie which mode we want to change to
int check_heating(float actual_temperature) {
  // Continue heating until the temperature rises above the target temperature
  if (actual_temperature < target_temperature) 
    return HEATING;
  return IDLE;
}

int check_cooling(float actual_temperature) {
  // Continue cooling until the temperature drops below the target temperature
  if (actual_temperature > target_temperature)
    return COOLING;
  return IDLE;
}

void loop_logic() {
  float actual_temperature = get_actual_temperature();
  
  // Record min/max temperatures
  if (actual_temperature < min_temperature)
    min_temperature = actual_temperature;
  if (actual_temperature > max_temperature)
    max_temperature = actual_temperature;

  long now = millis();

  // We want a pause between switching between cooling and heating
  switch (current_mode) {
  case HEATING: set_allowed_cool_time(now + COOL_HEAT_DELAY); break;
  case COOLING: set_allowed_heat_time(now + COOL_HEAT_DELAY); break;
  }
  
  // Determine desired mode
  switch (current_mode) {
  case IDLE:    desired_mode = check_idle(actual_temperature);    break;
  case HEATING: desired_mode = check_heating(actual_temperature); break;
  case COOLING: desired_mode = check_cooling(actual_temperature); break;
  }

  if (desired_mode == current_mode)
    return;
  
  // Set desired mode, if it is allowed
  switch (desired_mode) {
  case IDLE:    if (now > allowed_idle_time) set_mode(IDLE); break;
  case HEATING: if (now > allowed_heat_time) set_mode(HEATING); break;
  case COOLING: if (now > allowed_cool_time) set_mode(COOLING); break;
  }
}
