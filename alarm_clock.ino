#include "arduino_secrets.h"

// #include "RTC.h"

#include <R4SwRTC.h>
#define  TMR_FREQ_HZ  100.055  /* If swRTC goes forward, decrease the frequency, if it lags, increase the frequency */

r4SwRTC RTC; // Begin() called in network_time.ino
typedef struct tm tm_t;



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

bool alarmTriggered = false;
bool menuActive = false; 

unsigned long lastTick;
unsigned long lastMillis;
unsigned long overflow_offset = 0;

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

  // Get network time
  time_t unixTime = 0;

  bool network_success = get_network_time(&unixTime);
  if (!network_success) {
    Serial.println("Network time not set, defaulting to midnight");
  }

  // Start software RTC
  bool success = RTC.begin(TMR_FREQ_HZ);
  if (!success) {
    Serial.println("Failed to start RTC timer");
    while (true) delay (100);
  }

  RTC.setUnixTime(unixTime);
  Serial.println("The RTC was just set to: " + String(asctime(RTC.getTmTime())));

  // Display time on screen
  lcd.begin(16, 2);

  time_t currentTime = RTC.getUnixTime();
  displayTime(currentTime);

  lastMillis = millis();
}

void alarmCallback() {
  alarmTriggered = true;
}


#include "menu.h"

void loop() {
  
  // Begin running Menu, and then clean up after close
  if (menuActive) {
    menu::root::run(alarms, alarmMax);

    menuActive = false;
    
    time_t currentTime = RTC.getUnixTime();
    displayTime(currentTime);
  }

  // Set menu state
  if (button::pressed(button::MENU)) {
    Serial.println("(Pressed MENU): ");
    menuActive = true;
  }

  // If not in menu, update clock

  // Check for millis wraparound
  if (millis() < lastMillis) {
    lastMillis = millis();
    overflow_offset = lastTick + 1000; // Overflows to a value < 1000
    lastTick = 0;
  }

  // Every second on clock tick
  if (millis() - lastTick > 1000 - overflow_offset) {
    // Handle millis() overflow
    if (overflow_offset != 0) {
      lastTick = overflow_offset;
      overflow_offset = 0;
    }

    lastTick += 1000;
    time_t now = RTC.getUnixTime();
    tm_t currentTime = {0};
    gmtime_r(&now, &currentTime);
    
    // Update screen if new minute
    if (currentTime.tm_sec == 0) {
      // Update from NTP @ 4:30 am
      if (currentTime.tm_hour == 4 && currentTime.tm_min == 30) {
        // get_network_time() has the potential to lock up if wifi or ntp doesn't work. Need to ensure time keeps running even if 
        time_t network_time = 0;
        bool network_success = get_network_time(&network_time);
        if (network_success) {
          now = network_time;
          RTC.setUnixTime(now);
        }
      }
      displayTime(now);
      checkAlarms(alarms, alarmCount, now);
    }

    lastMillis = millis();
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
    time_t currentTime = RTC.getUnixTime();
    displayTime(currentTime);
  }
}

// This will like need to be modified to handle checks for repeating alarms
void checkAlarms(Alarm *alarm, int alarmCount, time_t currentTime) {
  for (int index = 0; index < alarmCount; index += 1) {

    if (alarm[index].enabled && alarm[index].isNow(currentTime)) {
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


void displayTime(time_t time) {
  char time_str[8];
  timeString(time, time_str);
  char date_str[17];
  dateString(time, date_str);

  // Write to screen
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print(time_str);
  lcd.setCursor(0, 1);
  lcd.print(date_str);

  // Write to serial monitor
  Serial.print("Time is ");
  Serial.print(String(time_str) + " (" + String((unsigned long)time) +")");
  Serial.println("");
}

void dateString(time_t time_val, char* date_str) {
  tm_t time;
  gmtime_r(&time_val, &time);

  strftime(date_str, 17, "%a, %e %b %Y", &time);
}


void timeString(time_t time_val, char* time_str) {
  time_str = strncpy(time_str, "00:00AM", 8);

  tm_t time;
  gmtime_r(&time_val, &time);
  
  int hour = time.tm_hour;
  int hour12 = (hour - 1) % 12 + 1;
  int minute = time.tm_min;

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



