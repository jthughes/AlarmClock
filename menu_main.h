#ifndef MENU_MAIN_H
#define MENU_MAIN_H

namespace root {
  void displayMenuMain(int index) {
    if (index == 0) {
      Serial.println("> New Alarm");
    } else {
      Serial.print("> " + String(index) + ": ");
      RTCTime timeToPrint = RTCTime(alarms[index - 1].time);
      writeTime(timeToPrint);
      if (alarms[index - 1].label) {
        Serial.print(" (");
        Serial.print(alarms[index - 1].label);
        Serial.print(")");
      }
      Serial.println("");
    }
  }

  void runMainMenu() {
    int index = 0;
    bool menuActive = true;
    displayMenuMain(index);
    while (menuActive) {
      if (button::pressed(button::NEXT)) {
        Serial.print("(Pressed NEXT): ");
        index += 1;
        if (index > 20 || alarms[index -1].set == false) {
          index = 0;
        }
        displayMenuMain(index);
      }

      if (button::pressed(button::SELECT)) {
        Serial.print("(Pressed SEL ): ");
        menu::alarm_time::runMenuAlarmSetTime(index);
        menuActive = false;
      }

      if (button::pressed(button::MENU)) {
        Serial.print("(Pressed MENU): ");
        menuActive = false;
        break ;
      }
    }
    Serial.println("Exiting main menu");
  }
}


#endif // MENU_MAIN_H