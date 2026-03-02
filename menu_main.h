#ifndef MENU_MAIN_H
#define MENU_MAIN_H

namespace root {
  void display(int index) {
    String message = "";
    if (index == 0 && alarmCount != alarmMax) {
      message = "> New Alarm";
    } else {
      time_t timeToPrint = alarms[index - 1].time;
      char time_str[8];
      timeString(timeToPrint, time_str);

      message = "> " + String(index) + ": " + String(time_str) + " [";
      if (alarms[index - 1].enabled) {
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

  void run(Alarm *alarms, int maxAlarms) {
    int index = 0;
    bool menuActive = true;
    display(index);
    while (menuActive) {
      if (button::pressed(button::NEXT)) {
        index += 1;
        if (index > 20 || alarms[index - 1].set == false) {
          index = 0;
        }
        display(index);
      }

      if (button::pressed(button::SELECT)) {
        Alarm tempAlarm;
        if (index == 0) { // New Alarm
          menu::alarm_time::run(&tempAlarm);
          if (menu::save_result::run()) {
            tempAlarm.set = true;
            tempAlarm.enabled = true;
            alarms[alarmCount] = tempAlarm;
            alarmCount++;
          } 
        } else { // Existing Alarm
          tempAlarm = alarms[index - 1];
          bool modified = menu::alarm_modify::run(&tempAlarm);
          if (modified && menu::save_result::run()) {
            tempAlarm.set = true;
            alarms[index - 1] = tempAlarm;
          }
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