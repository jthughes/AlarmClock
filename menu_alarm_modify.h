#ifndef MENU_ALARM_MODIFY_H
#define MENU_ALARM_MODIFY_H

#include "alarm.h"

namespace alarm_modify {

  enum menuItem {
    SET_TIME,
    SET_DAYS,
    SET_REPEAT,
  };

  const int menuSize = 3;

  String menuText[menuSize] = {
    "TIME",
    "DAYS",
    "REPEAT"
  };

  void display(int menuState) {
    String message = "-> " + menuText[menuState];
    Serial.println(message);
    lcd.clear();
    lcd.print(message);
    lcd.setCursor(0, 1);
    lcd.print("MENU-NEXT-SELECT");
  }

  bool run(Alarm *tempAlarm) {
    int menuState = SET_TIME;
    bool menuActive = true;
    bool modified = false;
    
    display(menuState);

    while (menuActive) {
      if (button::pressed(button::NEXT)) {
        menuState = (menuState + 1) % menuSize;
        display(menuState);
      }

      if (button::pressed(button::SELECT)) {
        bool result = false;
        switch (menuState) {
          case SET_TIME: {
            result = menu::alarm_time::run(tempAlarm);
          }
          break;
          case SET_DAYS: {
            result = menu::alarm_days::run(tempAlarm);
          }
          break;
          case SET_REPEAT: {
            result = menu::alarm_repeat::run(tempAlarm);
          }
          break;
        }
        if (result == true) {
          modified = true;
        }
        display(menuState);
      }

      if (button::pressed(button::MENU)) {
        menuActive = false;
        Serial.println("Exiting configure alarm menu");
      }
    }
    return modified;
  }
}

#endif // MENU_ALARM_MODIFY_H