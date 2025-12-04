#ifndef MENU_H
#define MENU_H

namespace menu {
  namespace save_result {
    bool run() {
      bool save = false;
      bool menuActive = true;
      Serial.println(" STORE [DISCARD]");
      while (menuActive) {
        if (button::pressed(button::NEXT)) {
          save = !save;
          if (save) {
            Serial.println("[STORE] DISCARD");
          } else {
            Serial.println(" STORE [DISCARD]");
          }
        }
        if (button::pressed(button::SELECT)) {
          if (save) {
            Serial.println("Alarm is saved");
          } else {
            Serial.println("Alarm is discarded");
          }
          return save;
        }
      }
    }
  }
  
  #include "menu_alarm_repeat.h"
  #include "menu_alarm_days.h"
  #include "menu_alarm_time.h"
  #include "menu_alarm_modify.h"
  #include "menu_main.h"
}

#endif // MENU_H