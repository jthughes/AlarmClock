#ifndef MENU_ALARM_DAYS_H
#define MENU_ALARM_DAYS_H

namespace alarm_days {

  const int menuSize = 7;

  String menuText[menuSize] = {
    "Mon",
    "Tue",
    "Wed",
    "Thu",
    "Fri",
    "Sat",
    "Sun"
  };

  unsigned int selected = 0;

  void display(int menuState) {
    Serial.print("-> " + menuText[menuState]);
    if (selected >> menuState & 1) {
      Serial.println(" [*]");
    } else {
      Serial.println(" [ ]");
    }
  }

  void run(int alarmEntry) {
    int menuState = 0;
    bool menuActive = true;

    display(menuState);

    while (menuActive) {
      if (button::pressed(button::NEXT)) {
        menuState = (menuState + 1) % menuSize;
        display(menuState);
      }

      if (button::pressed(button::SELECT)) {
        selected = selected | (1 << menuState);
        display(menuState);
      }

      if (button::pressed(button::MENU)) {
        menuActive = false;
      }
    }
    Serial.println("Exiting modify alarm days menu");
  }
}


#endif // MENU_ALARM_DAYS_H