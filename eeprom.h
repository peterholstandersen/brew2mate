// ==== Store/restore ==========================================================================

// float target_temperature       32-bit (but we only use XX.X)

#if 0

// For testing
byte eeprom_test[20] = { 0, };
#define EEPROM_WRITE(x,y) eeprom_test[x] = y
#define EEPROM_READ(x)    eeprom_test[x]

#else

#define EEPROM_WRITE(x,y) if (EEPROM.read(x) != y) EEPROM.write(x,y)
#define EEPROM_READ(x)    EEPROM.read(x)

#endif

int store_int(int address, int value)
{
  
  EEPROM_WRITE(address, value >> 8);   address++;
  EEPROM_WRITE(address, value & 0xff); address++;
  return address;
}

int read_int(int address, int *pValue)
{
  byte high = EEPROM_READ(address++);
  byte low  = EEPROM_READ(address++);
  *pValue = ((int)high << 8) | (int)low;
  return address;
}

int read_temperature(int address, float *pValue)
{
  int temperature_int;

  address = read_int(address, &temperature_int);
  *pValue = (float)temperature_int / 10.0;
  return address;
}

int dump_eeprom() {
  int i;

  for (i = 0; i < 20; i++) {
    Serial.print(i);
    Serial.print("  ");
    Serial.println(EEPROM_READ(i));
  }
}

void store_data() {
  int address = 0;

  address = store_int(address, round(target_temperature * 10.0));
}

void restore_data() {
  int address = 0;

  address = read_temperature(address, &target_temperature);
}


void loop_eeprom() {
  if (!PERSIST_DATA)
    return;
  
  static float stored_temperature = 0;

  const long eeprom_interval = 5000; // TODO: change once done testing
  static long next_eeprom_time = -1;
  long now = millis();
  if (now < next_eeprom_time) {
    return;
  }
  next_eeprom_time = now + eeprom_interval;

  float temp_diff = target_temperature - stored_temperature;
  if (temp_diff < 0)
    temp_diff = -temp_diff;
  if (temp_diff < 0.05)
    return;

  Serial.print("loop_eeprom: store_data: ");
  Serial.print(target_temperature, 1);
  Serial.print("\n");

  store_data();
  stored_temperature = target_temperature;
}
