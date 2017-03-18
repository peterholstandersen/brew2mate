// -*- mode: c -*-

#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ==== "Global" parameters and variables =====================================================

#define SCREENSAVER_DELAY         600000L   // 10 minutes

#define IDLE    1
#define HEATING 2
#define COOLING 3

#define TEMPERATURE_TOLERANCE        1.0f

#define START_HEATING_DELAY       300000L   // 5 minutes
#define START_COOLING_DELAY       300000L   // 5 minutes

#define MIN_HEATING_TIME           60000L   // 1 minute
#define MIN_COOLING_TIME           60000L   // 1 minute

const float target_temperature = 76.0;

float min_temperature = 99.9;
float max_temperature = 00.0;

int current_mode = IDLE;
int desired_mode = IDLE;

#include "common.h"
#include "display.h"
#include "one_wire.h"
#include "relays.h"
#include "logic.h"
#include "keys.h"
// #include "eeprom.h"
#include "logging.h"


// ==== Main ===================================================================================

void setup() {
  Serial.begin(115200);

  Serial.println("Brew2mate");
  
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.print(__TIME__);
  Serial.print(" ");
  Serial.print(__FILE__);
  Serial.print("\n");

  // print_boolean("PERSIST_DATA", PERSIST_DATA);
  print_float("TEMPERATURE_TOLERANCE", TEMPERATURE_TOLERANCE);
  print_long("START_HEATING_DELAY", START_HEATING_DELAY);
  print_long("START_COOLING_DELAY", START_COOLING_DELAY);
  print_long("MIN_HEATING_TIME", MIN_HEATING_TIME);
  print_long("MIN_COOLING_TIME", MIN_COOLING_TIME);
	     
  Serial.print("\n");

  setup_one_wire();
  setup_relays();
  setup_display();

  // restore_data();
}

void loop() {
  loop_keys();
  loop_one_wire();
  log_status();
  loop_logic();
  // loop_eeprom();
  display_status();
}
