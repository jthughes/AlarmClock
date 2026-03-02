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
    String message = "-> " + menuText[menuState];
    if (tempAlarm->isDaySet((day_t)menuState)) {
      message += " [*]";
    } else {
      message += " [ ]";
    }

    Serial.println(message);
    lcd.clear();
    lcd.print(message);
    lcd.setCursor(0, 1);
    lcd.print("MENU-NEXT-SELECT");

  }

  bool run(Alarm *tempAlarm) {
    int menuState = 0;
    bool menuActive = true;
    unsigned int initial_days = tempAlarm->getDays();

    display(tempAlarm, menuState);

    while (menuActive) {
      if (button::pressed(button::NEXT)) {
        menuState = (menuState + 1) % menuSize;
        display(tempAlarm, menuState);
      }

      if (button::pressed(button::SELECT)) {
        tempAlarm->toggleDay((day_t) menuState);
        display(tempAlarm, menuState);
      }

      if (button::pressed(button::MENU)) {
        menuActive = false;
      }
    }
    Serial.println("Exiting modify alarm days menu");
    bool modified = tempAlarm->getDays() != initial_days;
    return modified;
  }
}


#endif // MENU_ALARM_DAYS_H