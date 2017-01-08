/*  Digital Arduino Clock project.
 *  Kamil Kasperski, University of Silesia
 *
 *  This is my implementation of digital clock with chiptune melody based on Squawk library.
 *  It uses DS1307 RTC module to measure time even if battery power is not present
 *  That prevents clovk from restarting when the battery is low.
 *
 *  The code is optimized for 3.3V @ 16MHz ATMega328P based Arduino.
 *  It can work with 8MHz clock to save power but that would affect the melody playback.
 *
 *  Program uses my library for menus which is unfinished for now.
 *  Supports only Arduino IDE ver. 1.6.9 due to Squawk library incopatibility with newer versions.
 */

#include "LowPower.h"
#include "RTClib.h"
#include "MyMenu.h"
#include "Squawk.h"
#include <Wire.h>
#include <LiquidCrystal.h>

#define RIGHT   0
#define UP      1
#define DOWN    2
#define SELECT  3
#define NONE    4
//#define BACKLIGHT 11
#define BUZZPIN 2

LiquidCrystal lcd(13,12,8,9,10,11); // RS, E, D4, D5, D6, D7
RTC_DS1307 rtc;
SQUAWK_CONSTRUCT_ISR(SQUAWK_PWM_PIN3);

extern Melody alarm[];    // Castlevania melody compatible with Squawk

// menu fields - library still unfinished but usable
String main_fields[] = {"Ustaw godzine ", "Ustaw date", "Alarmy", "Status baterii", "O programie"};
void (*main_options[])() = {setTime, setDate, alarms, batteryStatus, about};    // Array of pointers to void functions. Works as I wanted
MyMenu main_menu("Menu glowne:", 5, main_fields);

String alarm_fields[] = {"Ustaw alarm", "Wlacz/Wylacz"};
void(*alarm_options[])() = {setAlarmMenu, turnAlarms};
MyMenu alarm_menu("Alarmy:", 2, alarm_fields);

// alarm options - can add as many alarms as I want. Just need to add more fields
DateTime _alarmy[3] = {DateTime(0,0,0,0,0,0), DateTime(0,0,0,0,0,0), DateTime(0,0,0,0,0,0)};
bool _alarmyEnabled[3] = {false, false, false};
String alarmy[] = {"Alarm 1", "Alarm 2", "Alarm 3"};

// external declarations for LCD icons
extern byte arrow_right[];
extern byte bell[];
extern byte square_chr[];
extern byte heart_on[];
extern byte battery_full[];
extern byte battery_mid_high[];
extern byte battery_mid[];
extern byte battery_low[];

int32_t delay_time;   // when this value is lower that actual time the clock goes to sleep
float battery_voltage;

void setup() {
  lcd.begin(16,2);
  pinMode(2, INPUT_PULLUP);

  lcd.createChar(0, battery_full);
  lcd.createChar(1, arrow_right);
  lcd.createChar(2, square_chr);
  lcd.createChar(3, heart_on);
  lcd.createChar(4, bell);
  lcd.createChar(5, battery_mid_high);
  lcd.createChar(6, battery_mid);
  lcd.createChar(7, battery_low);

  if (!rtc.begin()) {   // information when RTC is not present
    lcd.print("no RTC found");
    delay(2000);
  }
  if (!rtc.isrunning()) {   // inform that RTC is not working (or battery is dead...) and set default time in it.
    lcd.print("RTC not running");
    rtc.adjust(DateTime(2005, 4, 2, 21, 37, 0));
    delay(2000);
  }
  lcd.clear();
  delay_time = rtc.now().unixtime() + 60;
}

void loop() {
  DateTime now = rtc.now();
  if (now.unixtime() > delay_time) {    // sleep arduino for 8 seconds and after that do one iteration of loop() then sleep if actual time is higher
    digitalWrite(3, LOW);   // turn off buzzer for power saving purposes
    attachInterrupt(0, wakeUp, LOW);    // attach wake up event to INT0
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);   //low power function for energy saving purposes
    detachInterrupt(0);   // detach it - it is needed only in sleep
  }
  int btn = btn_read();

  // every iteration it checks each alarm if it meets the requirements to trigger
  for (int i = 0; i < sizeof(_alarmyEnabled); i++) {
    if ((_alarmy[i].hour() == now.hour()) and (_alarmy[i].minute() == now.minute())) {
      if (_alarmyEnabled[i]) {
        digitalWrite(3, HIGH);
        Squawk.begin(44100);
        Squawk.play(alarm);
        Squawk.tune(2.0);
        triggerAlarm(i);
        lcd.clear();
        digitalWrite(3, LOW);
        break;
      }
    }
  }

  lcd.setCursor(14,0);
  if ((_alarmyEnabled[0]) or (_alarmyEnabled[1]) or (_alarmyEnabled[2])) lcd.write(byte(4));

  lcd.setCursor(15,0);
  battery_voltage = voltage_avg();
  lcd.write(battery_percent(battery_voltage));

  lcd.setCursor(0, 0);
  if (now.second() % 2 == 0) {    //blinking hour spacer option instead of seconds
    lcd.print(show_hour_noSecs(now.hour(), now.minute(), ' '));   // 00 00
  } else {
    lcd.print(show_hour_noSecs(now.hour(), now.minute(), ':'));   // 00:00
  }

  lcd.setCursor(0, 1);
  lcd.print(show_date(now.day(), now.month(), now.year()));

  if ((btn != NONE) or (!digitalRead(BUZZPIN) == true)) {
    delay_time = now.unixtime() + 60;
  }

  switch (btn) {    // switch definition left just in case adding new features
    case SELECT: {
      lcd.clear();
      genMenu(&main_menu, main_options);
      lcd.clear();
      break;
    }
  }
}
