#ifndef MENU_MAIN_H
#define MENU_MAIN_H

namespace root {
  void display(int index) {
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

  void run() {
    int index = 0;
    bool menuActive = true;
    display(index);
    while (menuActive) {
      if (button::pressed(button::NEXT)) {
        index += 1;
        if (index > 20 || alarms[index -1].set == false) {
          index = 0;
        }
        display(index);
      }

      if (button::pressed(button::SELECT)) {
        if (index == 0) {
          menu::alarm_time::run(index);
        } else {
          menu::alarm_modify::run(index);
        }
        
        menuActive = false;
      }

      if (button::pressed(button::MENU)) {
        menuActive = false;
        break ;
      }
    }
    Serial.println("Exiting main menu");
  }
}


#endif // MENU_MAIN_H