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

// Button Pins: (   8,    9,  10)
// Function:    (MENU, NEXT, SEL)
const int btnCount = 3;
int btnPin[btnCount] = {8, 9, 10};
#include "button.h"

#include <SoftwareSerial.h>
SoftwareSerial mp3(6, 7);

#include "alarm.h"

const int alarmMax = 20;
Alarm alarms[alarmMax];
int alarmCount = 0;


bool tick;
bool alarmTriggered = false;
bool menuActive = false; 

void setup() {
  // Initialise buttons
  pinMode(btnPin[button::NEXT], INPUT_PULLUP);
  pinMode(btnPin[button::SELECT], INPUT_PULLUP);
  pinMode(btnPin[button::MENU], INPUT_PULLUP);

  // Initialise mp3 board
  mp3.begin(9600);

  // Initialise Serial Logging
  Serial.begin(9600);
  while (!Serial);

  // Initialise network time
  get_network_time();
  tick = true;
  RTC.setPeriodicCallback(clockTick, Period::ONCE_EVERY_1_SEC);

  // Initialise LCD screen
  lcd.begin(16, 2);


  RTCTime currentTime;
  RTC.getTime(currentTime);
  displayTime(currentTime);
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
    displayTime(currentTime);
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

      displayTime(currentTime);
      checkAlarms(alarms, alarmCount);
    }
  }

  // Detect alarm
  if (alarmTriggered) {
    alarmTriggered = false;
    Serial.println("Alarm!");
    
    // Write to screen
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("{{( ALARM! )}}");
    lcd.setCursor(0, 1);
    lcd.print("  CLR:Vol-:CLR  ");

    bool menuActive = true;
    while (menuActive) {
      if (button::pressed(button::NEXT)) {
        // Volume -
        byte buff[] = {0x7E, 0xFF, 0x06, 0x05, 0x00, 0x00, 0x00, 0xFE, 0xF6, 0xEF};
        mp3.write(buff, 10);
      }

      if (button::pressed(button::MENU) || button::pressed(button::SELECT)) {
        // Pause
        byte buff[] = {0x7E, 0xFF, 0x06, 0x0E, 0x00, 0x00, 0x00, 0xFE, 0xED, 0xEF};
        mp3.write(buff, 10);

        menuActive = false;
        break ;
      }
    }
    RTCTime currentTime;
    RTC.getTime(currentTime);
    displayTime(currentTime);
  }
}

// This will like need to be modified to handle checks for repeating alarms
void checkAlarms(Alarm *alarm, int alarmCount) {
  for (int index = 0; index < alarmCount; index += 1) {

    if (alarm[index].enabled && alarm[index].isNow()) {
      alarmTriggered = true;
      Serial.print("Alarm ");
      Serial.print(index + 1);
      Serial.println(" is ringing!");

      // Play Track 1
      byte buff[] = {0x7E, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x01, 0xFE, 0xF7, 0xEF};
      mp3.write(buff, 10);
      break ;
    }
  }
} 


void displayTime(RTCTime time) {
  char time_str[8];
  timeString(time, time_str);

  // Write to screen
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print(time_str);

  // Write to serial monitor
  Serial.print("Time is ");
  Serial.print(String(time_str));
  Serial.println("");
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



