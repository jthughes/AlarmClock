#ifndef MENU_ALARM_REPEAT_H
#define MENU_ALARM_REPEAT_H

namespace alarm_repeat {

  int menuSize = 2;
  enum menuItem {
    REPEAT_COUNT,
    REPEAT_DELAY,
  };

  const int repeatMax = 5;

  int repeatCount = 0;

  int repeatDelay = 5;

  void display(int menuState) {
    if (menuState == REPEAT_COUNT) {
      Serial.print("-> Repeat: ");
      Serial.println(repeatCount);
    } else if (menuState == REPEAT_DELAY) {
      Serial.print("-> Delay (min): ");
      Serial.println(repeatDelay);
    }
  }

  void run(int alarmEntry) {
    int menuState = REPEAT_COUNT;
    bool menuActive = true;

    display(menuState);

    while (menuActive) {
      if (button::pressed(button::NEXT)) {
        if (repeatCount > 0) {
          menuState = (menuState + 1) % menuSize;
        }
        display(menuState);
      }

      if (button::pressed(button::SELECT)) {
        if (menuState == REPEAT_COUNT) {
          repeatCount = (repeatCount + 1) % repeatMax;
        } else if (menuState == REPEAT_DELAY) {
          repeatDelay = repeatDelay % 30 + 5;
        }
        display(menuState);
      }

      if (button::pressed(button::MENU)) {
        menuActive = false;
      }
    }
    Serial.println("Exiting modify alarm repeat menu");
  }
}


#endif // MENU_ALARM_REPEAT_H