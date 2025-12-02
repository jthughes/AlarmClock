#ifndef MENU_H
#define MENU_H

namespace menu {

  bool runMenuSaveResult() {
    bool save = false;
    bool menuActive = true;
    Serial.println(" STORE [DISCARD]");
    while (menuActive) {
      if (button::pressed(button::NEXT)) {
        Serial.print("(Pressed NEXT): ");
        save = !save;
        if (save) {
          Serial.println("[STORE] DISCARD");
        } else {
          Serial.println(" STORE [DISCARD]");
        }
      }
      if (button::pressed(button::SELECT)) {
        Serial.print("(Pressed SEL ): ");
        if (save) {
          Serial.println("Alarm is saved");
        } else {
          Serial.println("Alarm is discarded");
        }
        return save;
      }
    }
  }
  #include "menu_alarm_time.h"
  #include "menu_main.h"
}

#endif // MENU_H