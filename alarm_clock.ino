#include "arduino_secrets.h"

#include "RTC.h"


#include <LiquidCrystal.h>


// Wiring guide
// GND: 1, 3, 5, 15 (Vgnd, Contrast, RW, LED-)
//  5V: 2, 16 (Vin, LED+)
// N/A: 7, 8, 9, 10 (d0, d1, d2, d3)


// LCD Pins:     ( 4,  6, 11, 12, 13, 14)
// Function:     (rs, en, d4, d5, d6, d7)
LiquidCrystal lcd(12, 11,  5,  4,  3,  2);




bool tick;
bool alarmTriggered;
bool menuActive; 

#include "alarm.h"

const int alarmMax = 20;
Alarm alarms[alarmMax];
int alarmCount = 2;

// 4x4 Matrix
const int btnCount = 3;
int btnPin[btnCount] = {8, 9, 10};
#include "button.h"


#include <SoftwareSerial.h>

SoftwareSerial mp3(6, 7);


void setup() {
  mp3.begin(9600);
  Serial.begin(9600);
  while (!Serial);

  get_network_time();

  tick = true;
  RTC.setPeriodicCallback(clockTick, Period::ONCE_EVERY_1_SEC);

  lcd.begin(16, 2);

  RTCTime currentTime;
  RTC.getTime(currentTime);
  writeTime(currentTime);
  Serial.println("");

  alarms[0] = Alarm(currentTime.getUnixTime() + 60, true, 0,300,0,"Monday");
  alarms[0].arm(alarmCallback);

  alarms[1] = Alarm(currentTime.getUnixTime() + 6000, true, 0, 300, 0, "Friday");
  alarms[1].arm(alarmCallback);

  alarmTriggered = false;

  menuActive = false;

  pinMode(btnPin[button::NEXT], INPUT_PULLUP);
  pinMode(btnPin[button::SELECT], INPUT_PULLUP);
  pinMode(btnPin[button::MENU], INPUT_PULLUP);
  
}

void alarmCallback() {
  alarmTriggered = true;
}

void clockTick() {
  tick = true;
}

#include "menu.h"

void loop() {
  
  // Begin running Menu, and then clean up after close
  if (menuActive) {
    menu::root::run(alarms, alarmMax);

    menuActive = false;
    RTCTime currentTime;
    RTC.getTime(currentTime);

    char time_str[8];
    timeString(currentTime, time_str);

    // Write to serial monitor
    Serial.print("Time is ");
    Serial.print(String(time_str));
    Serial.println("");

    // Write to screen
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print(time_str);

  }

  // Set menu state
  if (button::pressed(button::MENU)) {
    Serial.println("(Pressed MENU): ");
    menuActive = true;
  }

  // If not in menu, update clock
  if (tick) {
    tick = false;
    RTCTime currentTime;
    RTC.getTime(currentTime);
    if (currentTime.getSeconds() == 0) {
      char time_str[8];
      timeString(currentTime, time_str);

      // Write to serial monitor
      Serial.print("Time is ");
      Serial.print(String(time_str));
      Serial.println("");

      // Write to screen
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print(time_str);

      checkAlarms(alarms, alarmCount);
    }
  }

  // Detect alarm
  if (alarmTriggered) {
    alarmTriggered = false;
    Serial.println("Alarm!");
  }
}

// This will like need to be modified to handle checks for repeating alarms
void checkAlarms(Alarm *alarm, int alarmCount) {
  for (int index = 0; index < alarmCount; index += 1) {


    if (alarm[index].enabled && alarm[index].isNow()) {
      Serial.print("Alarm ");
      Serial.print(index + 1);
      Serial.println(" is ringing!");
      // Play?
      mp3.write(0x7E);
      mp3.write(0xFF);
      mp3.write(0x06);
      mp3.write(0x03);
      mp3.write((uint8_t)0x00);
      mp3.write((uint8_t)0x00);
      mp3.write(0x01);
      mp3.write(0xFE);
      mp3.write(0xF7);
      mp3.write(0xEF);
      break ;
    }
  }
} 



void writeTime(RTCTime time) {
  char time_str[8] = "00:00AM";
  

  int hour = time.getHour();
  int hour12 = (hour - 1) % 12 + 1;
  int minute = time.getMinutes();

  if (hour12 < 10) {
    time_str[1] += hour12;
  } else {
    time_str[0] += 1;
    time_str[1] += hour12 - 10;
  }

  if (minute < 10) {
    time_str[4] += minute;
  } else {
    time_str[3] = minute / 10 + '0';
    time_str[4] = minute % 10 + '0';
  }

  if (hour < 12) {
    time_str[5] = 'A';
  } else {
    time_str[5] = 'P';
  }

  
  lcd.print(time_str);

  Serial.print(String(time_str));
}


void timeString(RTCTime time, char* time_str) {
  time_str = strncpy(time_str, "00:00AM", 8);

  int hour = time.getHour();
  int hour12 = (hour - 1) % 12 + 1;
  int minute = time.getMinutes();

  if (hour12 < 10) {
    time_str[1] += hour12;
  } else {
    time_str[0] += 1;
    time_str[1] += hour12 - 10;
  }

  if (minute < 10) {
    time_str[4] += minute;
  } else {
    time_str[3] = minute / 10 + '0';
    time_str[4] = minute % 10 + '0';
  }

  if (hour < 12) {
    time_str[5] = 'A';
  } else {
    time_str[5] = 'P';
  }
}

