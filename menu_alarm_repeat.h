#ifndef MENU_ALARM_REPEAT_H
#define MENU_ALARM_REPEAT_H

#include "alarm.h"

namespace alarm_repeat {

  int menuSize = 2;
  enum menuItem {
    REPEAT_COUNT,
    REPEAT_DELAY,
  };

  


  void display(Alarm *tempAlarm, int menuState) {
    if (menuState == REPEAT_COUNT) {
      Serial.print("-> Repeat: ");
      Serial.println(tempAlarm->repeat);
    } else if (menuState == REPEAT_DELAY) {
      Serial.print("-> Delay (min): ");
      Serial.println(tempAlarm->repeat_delay_min);
    }
  }

  void run(Alarm *tempAlarm) {
    int menuState = REPEAT_COUNT;
    bool menuActive = true;

    display(tempAlarm, menuState);

    while (menuActive) {
      if (button::pressed(button::NEXT)) {
        if (tempAlarm->repeat > 0) {
          menuState = (menuState + 1) % menuSize;
        }
        display(tempAlarm, menuState);
      }

      if (button::pressed(button::SELECT)) {
        if (menuState == REPEAT_COUNT) {
          tempAlarm->repeat = (tempAlarm->repeat + 1) % tempAlarm->repeatMax;
        } else if (menuState == REPEAT_DELAY) {
          tempAlarm->repeat_delay_min = tempAlarm->repeat_delay_min % 30 + 5;
        }
        display(tempAlarm, menuState);
      }

      if (button::pressed(button::MENU)) {
        menuActive = false;
      }
    }
    Serial.println("Exiting modify alarm repeat menu");
  }
}


#endif // MENU_ALARM_REPEAT_H