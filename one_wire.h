// ==== 1-wire =================================================================================

#define TEMPERATURE_PROBE_INDEX    0
#define ONE_WIRE_PIN               2
#define TEMPERATURE_PRECISION     12

OneWire oneWire(ONE_WIRE_PIN);
DallasTemperature sensors(&oneWire);
DeviceAddress deviceAddress[4];
uint8_t deviceCount;
float actual_temperatures[4];

uint8_t probe1 = 10;
uint8_t probe2 = 10;
uint8_t probe3 = 10;
uint8_t probe4 = 10;

float get_actual_temperature() {
  // return actual_temperatures[TEMPERATURE_PROBE_INDEX];
  float probe1 = actual_temperatures[1];
  float probe3 = actual_temperatures[3];
  return (probe1 + probe2) / 2.0;
}

void setup_one_wire() {
  uint8_t i;

  sensors.begin();
  deviceCount = sensors.getDeviceCount();

  for (uint8_t i = 0; i < deviceCount; i++) {
    (void) sensors.getAddress(deviceAddress[i], i);
    switch (deviceAddress[i][1]) {
      case 0x29: probe1 = i; break;
      case 0xC0: probe2 = i; break;
      case 0xCC: probe3 = i; break;
      case 0x57: probe4 = i; break;
      default:
	break;
    }
    sensors.setResolution(deviceAddress[i], TEMPERATURE_PRECISION);
  }
  
  sensors.setWaitForConversion(false);
  sensors.requestTemperatures();
}

void loop_one_wire() {
  static long next_one_wire_time = -1;
  const long one_wire_interval = 1000;
  
  long now = millis();
  if (now < next_one_wire_time) {
    return;
  }
  next_one_wire_time = now + one_wire_interval;

  float temperature;
  
  for (uint8_t i = 0; i < deviceCount; i++) {
    temperature = sensors.getTempC(deviceAddress[i]);
  
    // A faulty reading is typically -127.0C. Only keep most sensible readings.
    if (0.0 <= temperature && temperature <= 110.0) {
      actual_temperatures[i] = temperature;
    }
  }

  // Requests temperatures (will take up to 750ms to be ready).
  // However, the call below is not blocking, but relies on the this function not being
  // called more often than every 750ms

  sensors.requestTemperatures();
}
