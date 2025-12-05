#ifndef MENU_ALARM_DAYS_H
#define MENU_ALARM_DAYS_H

#include "alarm.h"

namespace alarm_days {

  const int menuSize = 7;

  String menuText[menuSize] = {
    "Sun", // 0 days since Sunday to align with tm_wday
    "Mon", // 1
    "Tue", // 2
    "Wed", // 3
    "Thu", // 4
    "Fri", // 5
    "Sat"  // 6
  };

  void display(Alarm *tempAlarm, int menuState) {
    Serial.print("-> " + menuText[menuState]);
    if (tempAlarm->days >> menuState & 1) {
      Serial.println(" [*]");
    } else {
      Serial.println(" [ ]");
    }
  }

  void run(Alarm *tempAlarm) {
    int menuState = 0;
    bool menuActive = true;

    display(tempAlarm, menuState);

    while (menuActive) {
      if (button::pressed(button::NEXT)) {
        menuState = (menuState + 1) % menuSize;
        display(tempAlarm, menuState);
      }

      if (button::pressed(button::SELECT)) {
        tempAlarm->days = tempAlarm->days ^ (1 << menuState);
        display(tempAlarm, menuState);
      }

      if (button::pressed(button::MENU)) {
        menuActive = false;
      }
    }
    Serial.println("Exiting modify alarm days menu");
  }
}


#endif // MENU_ALARM_DAYS_H