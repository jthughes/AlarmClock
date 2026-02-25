#ifndef MENU_H
#define MENU_H

namespace menu {
  namespace save_result {
    bool run() {
      bool save = false;
      bool menuActive = true;
      String store_string   = "[STORE] DISCARD ";
      String discard_string = " STORE [DISCARD]";
      String current = discard_string;

      Serial.println(current);
      lcd.clear();
      lcd.print(current);
      lcd.setCursor(0,1);
      lcd.print("    -SWAP-SELECT");

      while (menuActive) {
        if (button::pressed(button::NEXT)) {
          save = !save;
          if (save) {
            current = store_string;
          } else {
            current = discard_string;
          }
          Serial.println(current);
          lcd.clear();
          lcd.print(current);
          lcd.setCursor(0,1);
          lcd.print("    -SWAP-SELECT");
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