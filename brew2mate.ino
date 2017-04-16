// -*- mode: c -*-

#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <limits.h>

// ==== "Global" parameters and variables =====================================================

#define TEST_MODE true

#define ONE_SECOND              1000L
#define ONE_MINUTE              (60L * ONE_SECOND)
#define THREE_MINUTES           ( 3L * ONE_MINUTE)
#define FIVE_MINUTES            ( 5L * ONE_MINUTE)
#define TEN_MINUTES             (10L * ONE_MINUTE)
#define FIFTEEN_MINUTES         (15L * ONE_MINUTE)
#define ONE_HOUR                (60L * ONE_MINUTE)
#define NEVERISH                LONG_MAX

#define SCREENSAVER_DELAY       TEN_MINUTES

#define IDLE    1
#define HEATING 2
#define COOLING 3

#define PROBE_INDEX1		1
#define PROBE_INDEX2		2

#define TEMPERATURE_TOLERANCE   0.5f

#if TEST_MODE
#define START_UP_DELAY          NEVERISH
#else
#define START_UP_DELAY          ONE_MINUTE
#endif

#define MINIMUM_IDLE_TIME       TEN_MINUTES
#define MINIMUM_HEAT_TIME       FIVE_MINUTES
#define MINIMUM_COOL_TIME       FIVE_MINUTES

// Time between switching from cooling to heating and vise versa
#define COOL_HEAT_DELAY         ONE_HOUR


#if !TEST_MODE
const
#endif
float target_temperature = 76.0;

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
#if TEST_MODE
#include "eeprom.h"
#endif
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

#if TEST_MODE
  Serial.print("TEST_MODE\n");
#endif
  print_float("TEMPERATURE_TOLERANCE", TEMPERATURE_TOLERANCE);
  print_long("START_UP_DELAY", START_UP_DELAY);
  print_long("MINIMUM_IDLE_TIME", MINIMUM_IDLE_TIME);
  print_long("MINIMUM_HEAT_TIME", MINIMUM_HEAT_TIME);
  print_long("MINIMUM_COOL_TIME", MINIMUM_COOL_TIME);
  print_long("COOL_HEAT_DELAY", COOL_HEAT_DELAY);
	     
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
