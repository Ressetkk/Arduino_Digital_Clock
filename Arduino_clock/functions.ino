int btn_read() {
  int btn = analogRead(A0);
  if (btn < 50) return UP;
  if (btn < 350) return DOWN;
  if (btn < 520) return RIGHT;
  if (btn < 620) return SELECT;
  return NONE;
}

// functions that return proper format for time and date
#define countof(a) (sizeof(a) / sizeof(a[0]))
String show_hour(uint8_t hours, uint8_t minutes, uint8_t seconds) {
  char timestr[20];
  snprintf_P(timestr,
             countof(timestr),
             PSTR("%02u:%02u:%02u"),
             hours,
             minutes,
             seconds );
  return timestr;
}
String show_hour_noSecs(uint8_t hours, uint8_t minutes, char comma) {
  char timestr[20];
  snprintf_P(timestr,
             countof(timestr),
             PSTR("%02u%c%02u"),
             hours,
             comma,
             minutes );
  return timestr;
}
String show_date(uint8_t days, uint8_t months, uint16_t years) {
  char datestr[20];
  snprintf_P(datestr,
             countof(datestr),
             PSTR("%02u/%02u/%04u"),
             days,
             months,
             years );
  return datestr;
}

// save prompt
byte savePrompt() {
  delay(250);
  byte decide = 3;
  while (btn_read() != SELECT) {
    lcd.setCursor(0, 0);
    lcd.print("Zapisz:");
    switch (btn_read()) {
      case RIGHT:  {
        lcd.setCursor(0,1);
        lcd.print("                ");
        decide += 6;
        if (decide > 9) decide = 3;
        break;
      }
    }
    lcd.setCursor(4, 1);
    lcd.print("Tak");
    lcd.setCursor(10, 1);
    lcd.print("Nie");
    lcd.setCursor(decide, 1);
    lcd.write(byte(1));
    delay(250);
  }
  return decide;
}

// Time set function
void setTime() {
  DateTime now = rtc.now();

  int8_t temp_hour = now.hour();
  int8_t temp_minute = now.minute();
  int8_t index = 1;

  delay(250);
  while (btn_read() != SELECT) {
    lcd.setCursor(0, 0);
    DateTime temp_time;
    lcd.print("Ustaw godzine:");
    lcd.setCursor(0, 1);
    lcd.print(show_hour_noSecs(temp_hour, temp_minute, ':'));
    lcd.setCursor(index, 1);
    lcd.blink();    // this is not properly made. Looking for a better solution for OLED screen.
    switch (btn_read()) {
      case RIGHT: if (index <= 4) index += 3; if (index > 4) index = 1; break;
      case UP: {
        switch (index) {
          case 1: if (temp_hour <= 24) temp_hour++; if (temp_hour == 24) temp_hour = 0; break;
          case 4: if (temp_minute <= 60) temp_minute++; if (temp_minute == 60) temp_minute = 0; break;
        }
        break;
      }
      case DOWN: {
        switch (index) {
          case 1: if (temp_hour >= 0) temp_hour--; if (temp_hour < 0) temp_hour = 23; break;
          case 4: if (temp_minute >= 0) temp_minute--; if (temp_minute < 0) temp_minute = 59; break;
        }
        break;
      }
    }
    delay(250);
  }
  lcd.noBlink();
  lcd.clear();
  byte decide = savePrompt();
  if (decide == 3) {
    rtc.adjust(DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), temp_hour, temp_minute, 0));
    lcd.setCursor(15, 0);
    lcd.write(byte(3));
    delay(2000);
  }
}

// Days of month setter
unsigned int setNumOfDays(uint8_t temp_month, uint16_t temp_year) {
  if ((temp_month == 1) or
      (temp_month == 3) or
      (temp_month == 5) or
      (temp_month == 7) or
      (temp_month == 8) or
      (temp_month == 10) or
      (temp_month == 12)) {
    return 31;
  }
  if ((temp_month == 4) or
      (temp_month == 6) or
      (temp_month == 9) or
      (temp_month == 11)) {
    return 30;
  }
  if (temp_month == 2) {
    if (((temp_year % 4 == 0) and (temp_year % 100 != 0)) or (temp_year % 400 == 0)) {
      return 29;
    } else return 28;
  }
}

// Date set function
void setDate() {
  DateTime now = rtc.now();
  uint16_t temp_year = now.year();
  uint8_t temp_month = now.month();
  uint8_t temp_day = now.day();

  unsigned int days_count = 31;
  int8_t index = 1;

  delay(250);
  while (btn_read() != SELECT) {
    lcd.setCursor(0, 0);
    lcd.print("Ustaw date:");
    lcd.setCursor(0, 1);
    lcd.print(show_date(temp_day, temp_month, temp_year));
    lcd.setCursor(index, 1);
    days_count = setNumOfDays(temp_month, temp_year);
    if (temp_day > days_count) temp_day = days_count;
    lcd.blink();    // this is not properly made. Looking for a better solution for OLED screen.
    switch (btn_read()) {
      case UP: {
          switch (index) {
            case 1: {
              if (temp_day <= days_count) temp_day++;
              if (temp_day > days_count) temp_day = 1;
              break;
            }
            case 4: {
              if (temp_month <= 12) temp_month++;
              if (temp_month > 12) temp_month = 1;
              break;
            }
            case 9: {
              if (temp_year <= 2099) temp_year++;
              if (temp_year > 2099) temp_year = 2000;
              break;
            }
          }
          break;
        }
      case DOWN: {
          switch (index) {
            case 1: {
                if (temp_day >= 1) temp_day--;
                if (temp_day < 1) temp_day = days_count;
                break;
              }
            case 4: {
                if (temp_month >= 1) temp_month--;
                if (temp_month < 1) temp_month = 12;
                break;
              }
            case 9: {
                if (temp_year >= 2000) temp_year--;
                if (temp_year < 2000) temp_year = 2099;
                break;
              }
          }
          break;
        }
      case RIGHT: {
          if (index <= 9) index += 3;
          if (index == 7) index = 9;
          if (index > 9) index = 1;
          break;
        }
    }
    delay(250);
  }
  lcd.noBlink();
  lcd.clear();
  byte decide = savePrompt();
  if (decide == 3) {
    rtc.adjust(DateTime(temp_year, temp_month, temp_day, rtc.now().hour(), rtc.now().minute(), rtc.now().second()));
    lcd.setCursor(15, 0);
    lcd.write(byte(3));
    delay(2000);
  }
}

// forwerder function to genMenu function - works somehow
void alarms() {
  genMenu(&alarm_menu, alarm_options);
}

// Alarm menu
void setAlarmMenu() {
  byte index = 0;
  delay(250);
  while (btn_read() != UP) {
    lcd.setCursor(0, 0);
    lcd.print(alarm_menu.showField());
    lcd.setCursor(15, 1);
    lcd.write(1);
    lcd.setCursor(0, 1);
    lcd.print(alarmy[index]);
    switch (btn_read()) {
      case RIGHT: lcd.setCursor(0,1); lcd.print("               "); index++; if (index >= sizeof(_alarmyEnabled)) index = 0; break;
      case SELECT: lcd.clear(); _alarmy[index] = setAlarm(index); lcd.clear(); break;
    }
    delay(250);
  }
}

DateTime setAlarm(byte _index) {
  int8_t hour = _alarmy[_index].hour();
  int8_t minute = _alarmy[_index].minute();
  bool alarmEnabled = false;

  byte index = 1;
  delay(250);
  while (btn_read() != SELECT) {
    lcd.setCursor(0, 0);
    lcd.print(alarmy[_index]);
    lcd.setCursor(15, 0);
    if (alarmEnabled == true) lcd.write(byte(4)); else lcd.print(" ");
    if (!digitalRead(2) == true) alarmEnabled = !alarmEnabled;
    lcd.setCursor(0, 1);
    lcd.print(show_hour_noSecs(hour, minute, ':'));
    lcd.setCursor(index, 1);
    lcd.blink();    // this is not properly made. Looking for a better solution for OLED screen.
    switch (btn_read()) {
      case RIGHT: index++; if (index == 2) index = 4; if (index > 4) index = 1; break;
      case UP: {
          switch (index) {
            case 1: hour++; if (hour == 24) hour = 0; break;
            case 4: minute++; if (minute == 60) minute = 0; break;
          }
          break;
        }
      case DOWN: {
          switch (index) {
            case 1: hour--; if (hour < 0) hour = 23; break;
            case 4: minute--; if (minute < 0) minute = 59; break;
          }
          break;
        }
    }
    delay(250);
  }
  lcd.noBlink();
  lcd.clear();
  byte decide = savePrompt();
  if (decide == 3) {
    lcd.setCursor(15, 0);
    lcd.write(byte(3));
    delay(2000);
    _alarmyEnabled[_index] = alarmEnabled;
    return DateTime(0, 0, 0, hour, minute, 0);
  }
}

// Show turn on / off alarms
void turnAlarms() {
  byte index = 0;
  delay(250);
  while (btn_read() != SELECT) {
    byte alarm_button = !digitalRead(2);
    lcd.setCursor(0, 0);
    lcd.print(alarmy[index]);
    lcd.setCursor(15, 0);
    if (!digitalRead(2) == 1) _alarmyEnabled[index] = !_alarmyEnabled[index];
    if (_alarmyEnabled[index] == true) lcd.write(byte(4)); else lcd.print(" ");
    lcd.setCursor(0, 1);
    lcd.print(show_hour_noSecs(_alarmy[index].hour(), _alarmy[index].minute(), ':'));
    switch (btn_read()) {
      case RIGHT: index++; if (index >= sizeof(_alarmyEnabled)) index = 0; break;
      case SELECT: _alarmyEnabled[index] = !_alarmyEnabled[index];
    }
    delay(250);
  }
}

// this is function that returns avg value of voltage based on 100 measurements
float voltage_avg() {
  float voltage;
  float voltage_sum = 0.0;
  for (int i = 0; i < 100; i++) {
    voltage = ((analogRead(A2) / 1023.0) * readVcc()) / 1000.0;
    voltage_sum += voltage;
  }
  return voltage_sum / 100.0;
}

// Battery status menu
// should have done capacity measure but this implementation also works OK - not 100% accurate but OK.
void batteryStatus() {
  delay(250);
  while (btn_read() != UP) {
    lcd.setCursor(0, 0);
    lcd.print("Status baterii:");
    lcd.setCursor(0, 1);
    float voltage = voltage_avg();
    lcd.print(voltage);
    lcd.print("V");
    lcd.setCursor(7,1);
    float proc_voltage = ((voltage - 0.80) * 100.0) / 2.51; 
    lcd.print(int(ceil(proc_voltage)));
    lcd.print("%");
    lcd.setCursor(15,1);
    lcd.write(battery_percent(voltage));
    delay(100);
  }
  lcd.clear();
}

// About
void about() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Digital Clock");
  lcd.setCursor(0, 1);
  lcd.print("Firm rev3 OLED");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Kamil");
  lcd.setCursor(0, 1);
  lcd.print("Kasperski");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Compiled on");
  lcd.setCursor(0,1);
  lcd.print(__DATE__);
  delay(2000);
}

// genMenu function which will be inside MyMenu library. For this project this is here.
void genMenu(MyMenu *menu, void (**option)()) {
  delay(250);
  while (btn_read() != UP) {
    int btn = btn_read();
    lcd.setCursor(0, 0);
    lcd.print(menu->getLabel());
    lcd.setCursor(15, 1);
    lcd.write(byte(1));
    lcd.setCursor(0, 1);
    lcd.setCursor(0, 1);
    lcd.print(menu->showField());
    switch (btn) {
      case RIGHT: lcd.setCursor(0,1); lcd.print("              "); menu->setNextField(); break;
      case SELECT: lcd.clear(); option[menu->getIndex()](); lcd.clear(); break;
    }
    delay(250);
    delay_time = rtc.now().unixtime() + 60;
  }
}

void triggerAlarm(byte index) {
  lcd.clear();
  lcd.setCursor(0, 1);
  while (digitalRead(2)) {
    lcd.setCursor(0, 0);
    lcd.print("Dzwoni alarm!");
    lcd.setCursor(0, 1);
    if (rtc.now().second() % 2 == 0) {
      lcd.print(alarmy[index]);
    } else lcd.print("                ");
    delay(250);
  }
  _alarmyEnabled[index] = !_alarmyEnabled[index];
  Squawk.stop();
}

// wake up event that does nothing because it has to do nothing to wake up clock from sleep.
void wakeUp () {
  
}

// function to measure voltage that flows through Arduino based on 1.1V internal reference for measuring purposes
// more info here http://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/
// code optimized for this exact project
long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both
  long scale_const = ((1.1 * 3310) / 3379) * 1023 * 1000;
  long result = (high<<8) | low;

  result = scale_const / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

// that function returns specific battery icon number to write on the screen according to given voltage
// optimized for 2 AA alkaline batteries connected in serial. (Duracell Industrial)
byte battery_percent(float voltage) {
  /*
   * 0 - 100%
   * 5 - 75%
   * 6 - 50%
   * 7 - 25%
   */

  if (voltage < 1.43) return 7;
  if (voltage < 2.06) return 6;
  if (voltage < 2.68) return 5;
  return 0;
}

