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
    if (menuState == SET_HOUR) {
      Serial.print("[");
    } else {
      Serial.print(" ");
    }

    if (time.hour < 10) {
      Serial.print(" ");
    }
    Serial.print(time.hour);

    if (menuState == SET_HOUR) {
      Serial.print("]");
    } else {
      Serial.print(" ");
    }

    
    Serial.print(":");
    

    if (menuState == SET_MINUTE) {
      Serial.print("[");
    }  else {
      Serial.print(" ");
    }

    if (time.minute < 10) {
      Serial.print("0");
    }
    Serial.print(time.minute);

    if (menuState == SET_MINUTE) {
      Serial.print("]");
    } else {
      Serial.print(" ");
    }


    if (menuState == SET_AFTERNOON) {
      Serial.print("[");
    } else {
      Serial.print(" ");
    }

    if (time.afternoon) {
      Serial.print("PM");
    } else {
      Serial.print("AM");
    }

    if (menuState == SET_AFTERNOON) {
      Serial.print("]");
    } else {
      Serial.print(" ");
    }

    Serial.println("");
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
    ClockTime time;

    RTCTime alarmTime;
    alarmTime.setUnixTime(tempAlarm->time);

    time.hour = alarmTime.getHour();
    time.minute = alarmTime.getMinutes();
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
        tempAlarm->time = time.minute*60 + time.hour*3600 +12*3600*time.afternoon;
      }
    }
    
  }
}


#endif // MENU_ALARM_TIME_H