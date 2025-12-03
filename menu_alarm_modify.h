#ifndef MENU_ALARM_MODIFY_H
#define MENU_ALARM_MODIFY_H

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

  void run(int alarmEntry) {
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
            menu::alarm_time::run(alarmEntry);
          }
          break;
          case SET_DAYS: {
            menu::alarm_days::run(alarmEntry);
          }
          break;
        }
      }

      if (button::pressed(button::MENU)) {
        menuActive = false;
        int result = menu::save_result::run();
        if (result) {
          if (alarmEntry == 0) {
            // alarms[alarmCount] = Alarm(time.minute*60 + time.hour*3600 +12*3600*time.afternoon);
            alarmCount++;
          }
        } 
      }
    }
    Serial.println("Exiting configure alarm menu");
  }
}


#endif // MENU_ALARM_MODIFY_H