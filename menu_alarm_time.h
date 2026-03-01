#ifndef MENU_ALARM_TIME_H
#define MENU_ALARM_TIME_H

#include "alarm.h"

namespace alarm_time {

  struct ClockTime {
    int hour = 12;
    int minute = 0;
    bool afternoon = false;
  };

  enum configAlarmMenu {
    SET_HOUR,
    SET_MINUTE,
    SET_AFTERNOON,
  };

  void display(int menuState, struct ClockTime time) {

    String message = "";
    if (menuState == SET_HOUR) {
      message += "[";
    } else {
      message += " ";
    }

    if (time.hour < 10) {
      message += " ";
    }
    message += String(time.hour);

    if (menuState == SET_HOUR) {
      message += "]";
    } else {
      message += " ";
    }

    message += ":";
    
    if (menuState == SET_MINUTE) {
      message += "[";
    }  else {
      message += " ";
    }

    if (time.minute < 10) {
      message += "0";
    }
    message += String(time.minute);

    if (menuState == SET_MINUTE) {
      message += "]";
    } else {
      message += " ";
    }


    if (menuState == SET_AFTERNOON) {
      message += "[";
    } else {
      message += " ";
    }

    if (time.afternoon) {
      message += "PM";
    } else {
      message += "AM";
    }

    if (menuState == SET_AFTERNOON) {
      message += "]";
    } else {
      message += " ";
    }

    Serial.println(message);
    lcd.clear();
    lcd.print(message);
    lcd.setCursor(0, 1);
    lcd.print("MENU-NEXT-SELECT");
  }


  // alarmEntry 0 means creating new alarm
  // alarmEntry 1+ means modify existing alarm
  // Validate on 1+ that alarm exists. Just throw error? Shouldn't ever be able to get into this state.


  // MENU button prompts to STORE or DISCARD
  //  - Changes to alarm or new alarm only set if STORED, otherwise memory not changed
  // If alarmEntry was not 0, so modifying existing Alarm, Options should include DELETE, with ARE YOU SURE: YES/NO prompt


  // New alarm should only show some options: Want to be able to set quickly. For more advanced use, modify after setting.
  bool run(Alarm *tempAlarm) {
    int menuState = SET_HOUR;
    bool menuActive = true;
    bool modified = false;
    ClockTime time;


    tm_t alarmTime = {0};
    gmtime_r(&tempAlarm->time, &alarmTime);

    time.hour = alarmTime.tm_hour;
    time.minute = alarmTime.tm_min;

    if (time.hour > 12) {
      time.hour -= 12;
      time.afternoon = true;
    }

    display(menuState, time);

    while (menuActive) {
      if (button::pressed(button::NEXT)) {

        switch (menuState) {
          case SET_HOUR:{
            time.hour = time.hour%12 + 1;
            display(menuState, time);
          }
          break;
          case SET_MINUTE: {
            time.minute = (time.minute + 5)%60;
            display(menuState, time);
          }
          break;
          case SET_AFTERNOON: {
            time.afternoon = !time.afternoon;
            display(menuState, time);
          }
          break;
          default:
            // You should not get here.
            break;
        }
      }

      if (button::pressed(button::SELECT)) {
        menuState = (menuState + 1) % 3;
        display(menuState, time);
      }

      if (button::pressed(button::MENU)) {
        menuActive = false;
        Serial.println("Exiting configure alarm menu");
        time_t initial_time = tempAlarm->time;
        tempAlarm->time = time.minute*60 + time.hour*3600 +12*3600*time.afternoon;
        modified = tempAlarm->time != initial_time;
      }
    }
    return modified;
  }
}


#endif // MENU_ALARM_TIME_H