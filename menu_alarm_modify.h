#ifndef MENU_ALARM_MODIFY_H
#define MENU_ALARM_MODIFY_H

#include "alarm.h"

namespace alarm_modify {

  enum menuItem {
    SET_ENABLED,
    SET_TIME,
    SET_DAYS,
    SET_REPEAT,
  };

  const int menuSize = 4;

  String menuText[menuSize] = {
    "ENABLED",
    "TIME",
    "DAYS",
    "REPEAT"
  };

  void display(int menuState, Alarm *tempAlarm) {
    String message = "-> " + menuText[menuState];
    if (menuState == menuItem::SET_ENABLED) {
      message += " [";
      if (tempAlarm->enabled) {
        message += "*";
      } else {
        message += " ";
      }
      message += "]";
    }

    Serial.println(message);
    lcd.clear();
    lcd.print(message);
    lcd.setCursor(0, 1);
    lcd.print("MENU-NEXT-SELECT");
  }

  bool run(Alarm *tempAlarm) {
    int menuState = SET_ENABLED;
    bool menuActive = true;
    bool modified = false;
    
    bool initial_status = tempAlarm->enabled;

    display(menuState, tempAlarm);

    while (menuActive) {
      if (button::pressed(button::NEXT)) {
        menuState = (menuState + 1) % menuSize;
        display(menuState, tempAlarm);
      }

      if (button::pressed(button::SELECT)) {
        bool result = false;
        switch (menuState) {
          case menuItem::SET_ENABLED: {
            tempAlarm->enabled = !tempAlarm->enabled;
          }
          break; 
          case menuItem::SET_TIME: {
            result = menu::alarm_time::run(tempAlarm);
          }
          break;
          case menuItem::SET_DAYS: {
            result = menu::alarm_days::run(tempAlarm);
          }
          break;
          case menuItem::SET_REPEAT: {
            result = menu::alarm_repeat::run(tempAlarm);
          }
          break;
        }
        if (result == true) {
          modified = true;
        }
        display(menuState, tempAlarm);
      }

      if (button::pressed(button::MENU)) {
        menuActive = false;
        Serial.println("Exiting configure alarm menu");
      }
    }
    if (tempAlarm->enabled != initial_status) {
      modified = true;
    }
    return modified;
  }
}

#endif // MENU_ALARM_MODIFY_H