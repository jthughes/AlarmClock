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
    Serial.println("-> " + menuText[menuState]);
  }

  bool run(Alarm *tempAlarm) {
    int menuState = SET_TIME;
    bool menuActive = true;

    display(menuState);

    while (menuActive) {
      if (button::pressed(button::NEXT)) {
        menuState = (menuState + 1) % menuSize;
        display(menuState);
      }

      if (button::pressed(button::SELECT)) {
        switch (menuState) {
          case SET_TIME: {
            menu::alarm_time::run(tempAlarm);
          }
          break;
          case SET_DAYS: {
            menu::alarm_days::run(tempAlarm);
          }
          break;
          case SET_REPEAT: {
            menu::alarm_repeat::run(tempAlarm);
          }
          break;
        }
      }

      if (button::pressed(button::MENU)) {
        menuActive = false;
        Serial.println("Exiting configure alarm menu");
      }
    }
    
  }
}


#endif // MENU_ALARM_MODIFY_H