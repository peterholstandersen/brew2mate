// ==== Display ================================================================================

LiquidCrystal lcd(4, 5, 6, 7, 8, 9);
long activate_screensaver_time = SCREENSAVER_DELAY;

void setup_display() {
  lcd.begin(20, 4);
}

void screensaver_off() {
  activate_screensaver_time = millis() + SCREENSAVER_DELAY;
}

void screensaver_on() {
  activate_screensaver_time = millis() - 1;
}

bool is_screensaver_active() {
  return millis() > activate_screensaver_time;
}

void toggle_screensaver() {
  if (is_screensaver_active())
    screensaver_off();
  else
    screensaver_on();
}

void display_integer(long number, char prefix) {
  int digit1 = (number / 10) % 10;
  int digit2 = number % 10;

  if (digit1 > 0)
    lcd.print(digit1);
  else
    lcd.print(prefix);

  if (digit2 > 0)
    lcd.print(digit2);
  else
    lcd.print(prefix);
}

long next_display_update = 0;

void display_normal_mode() {
  switch (current_mode) {
  case IDLE:    lcd.print("IDLE    "); break;
  case HEATING: lcd.print("HEATING "); break;
  case COOLING: lcd.print("COOLING "); break;
  }
}

void display_blinking_mode() {
  static int count = 0;
  static long next_change = 0;

  long now = millis();

  if (now > next_change) {
    count++;
    if (count % 2 == 1)
      next_change = now + 250;
    else
      next_change = now + 750;
  }

  if (count % 2 == 1)
    lcd.print("        ");
  else
    display_normal_mode();
}

void display_mode() {
  if (desired_mode != current_mode)
    display_blinking_mode();
  else
    display_normal_mode();
}  

void display_uptime() {
  long now = millis();
  long seconds = now / 1000;
  long minutes = seconds / 60;
  long hours = minutes / 60;
  long days = hours / 24;

  seconds = seconds % 60;
  minutes = minutes % 60;
  hours = hours % 24;

  display_integer(days, ' ');
  if (days > 0)
    lcd.print("d ");
  else
    lcd.print("  ");

  display_integer(hours, '0');
  lcd.print(":");
  display_integer(minutes, '0');
  lcd.print(":");
  display_integer(seconds, '0');
}

void display_line1() {
  lcd.setCursor(0, 0);
  display_mode();
  display_uptime();
}

void print_temperature(float temperature) {
  if (temperature < 10.0)
    lcd.print(" ");
  lcd.print(temperature, 1);
}

void display_screensaver() {
  static int line = 0;
  static int column = 0;
  static long next_update = 0;

  long now = millis();
  if (now < next_update)
    return;
  next_update = now + 100;
  
  lcd.clear();
  lcd.setCursor(column, line);
  lcd.print(".");

  column++;
  if (column >= 20) {
    column = 0;
    line++;
    line = line % 4;
  }
}

void display_status() {

  long now = millis();
  if (is_screensaver_active()) {
    display_screensaver();
    return;
  }
    
  
  // 01234567890123456789
  // --------------------
  // STATE   XXd xx:xx:xx
  // Target          xx.x
  // Actual          xx.x
  // Min/max    xx.x/xx.x
  // --------------------

  display_line1();

  lcd.setCursor(0, 1);
  lcd.print("Target          ");
  print_temperature(target_temperature);

  lcd.setCursor(0, 2);
  lcd.print("Actual          ");
  print_temperature(get_actual_temperature());

  lcd.setCursor(0, 3);
  lcd.print("Min/max    ");
  if (min_temperature < 10.0)
    lcd.print(" ");
  if (max_temperature < 10.0)
    lcd.print(" ");
  lcd.print(min_temperature, 1);
  lcd.print("/");
  lcd.print(max_temperature, 1);
}
