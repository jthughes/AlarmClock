#include "arduino_secrets.h"

#include "RTC.h"

// OLED
#include <U8x8lib.h>
U8X8_SSD1306_128X64_NONAME_4W_SW_SPI u8x8(/* clock=*/ 12, /* data=*/ 11, /* cs=*/ 8, /* dc=*/ 9, /* reset=*/ 10);

bool tick;
bool alarmTriggered;
bool menu; 

#include "alarm.h"

Alarm alarms[20];
int alarmCount = 2;

// 4x4 Matrix
const int btnCount = 3;
int btnPin[btnCount] = {4, 3, 2};
#include "button.h"



void setup() {
  Serial.begin(9600);
  while (!Serial);

  get_network_time();

  tick = true;
  RTC.setPeriodicCallback(clockTick, Period::ONCE_EVERY_1_SEC);

  u8x8.begin();
  RTCTime currentTime;
  RTC.getTime(currentTime);
  writeTime(currentTime);

  alarms[0] = Alarm(currentTime.getUnixTime() + 600, true, 0,300,0,"Monday");
  alarms[0].arm(alarmCallback);

  alarms[1] = Alarm(currentTime.getUnixTime() + 6000, true, 0, 300, 0, "Friday");
  alarms[1].arm(alarmCallback);

  alarmTriggered = false;

  menu = false;

  pinMode(btnPin[button::NEXT], INPUT);
  pinMode(btnPin[button::SELECT], INPUT);
  pinMode(btnPin[button::MENU], INPUT);
  
}

void alarmCallback() {
  alarmTriggered = true;
}

void clockTick() {
  tick = true;
}

void loop() {
  
  if (menu) {
    viewAlarms();
    menu = false;
    RTCTime currentTime;
    RTC.getTime(currentTime);
    writeTime(currentTime); 
    Serial.println("");
  }

  if (button::pressed(button::MENU)) {
    Serial.println("(Pressed MENU): ");
    menu = true;
  }

  if (tick) {
    tick = false;
    RTCTime currentTime;
    RTC.getTime(currentTime);
    if (currentTime.getSeconds() == 0) {
      Serial.print("Time is "); 
      writeTime(currentTime);
      Serial.println("");
    }
  }

  if (alarmTriggered) {
    alarmTriggered = false;
    Serial.println("Alarm!");
  }
}

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

void viewAlarms() {
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
      configureAlarm(index);
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



struct ClockTime {
  int hour = 12;
  int minute = 0;
  bool afternoon = false;
};


  enum configAlarmMenu {
    SET_HOUR,
    SET_MINUTE,
    SET_AFTERNOON,
    STORE_RESULT
  };

void displayConfigAlarm(int menuState, struct ClockTime time) {
  if (menuState == SET_HOUR) {
    Serial.print("[");
  } else {
    Serial.print(" ");
  }

  if (time.hour < 10) {
    Serial.print(" ");
  }
  Serial.print(time.hour);

  if (menuState == SET_HOUR) {
    Serial.print("]");
  } else {
    Serial.print(" ");
  }

  
  Serial.print(":");
  

  if (menuState == SET_MINUTE) {
    Serial.print("[");
  }  else {
    Serial.print(" ");
  }

  if (time.minute < 10) {
    Serial.print("0");
  }
  Serial.print(time.minute);

  if (menuState == SET_MINUTE) {
    Serial.print("]");
  } else {
    Serial.print(" ");
  }


  if (menuState == SET_AFTERNOON) {
    Serial.print("[");
  } else {
    Serial.print(" ");
  }

  if (time.afternoon) {
    Serial.print("PM");
  } else {
    Serial.print("AM");
  }

  if (menuState == SET_AFTERNOON) {
    Serial.print("]");
  } else {
    Serial.print(" ");
  }

  Serial.println("");
}


bool menuSaveResult() {
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


void configureAlarm(int alarmEntry) {
  // alarmEntry 0 means creating new alarm
  // alarmEntry 1+ means modify existing alarm
  // Validate on 1+ that alarm exists. Just throw error? Shouldn't ever be able to get into this state.


  // MENU button prompts to STORE or DISCARD
  //  - Changes to alarm or new alarm only set if STORED, otherwise memory not changed
  // If alarmEntry was not 0, so modifying existing Alarm, Options should include DELETE, with ARE YOU SURE: YES/NO prompt


  // New alarm should only show some options: Want to be able to set quickly. For more advanced use, modify after setting.

  



  int menuState = SET_HOUR;
  bool menuActive = true;
  ClockTime time;

  displayConfigAlarm(menuState, time);

  while (menuActive) {
    if (button::pressed(button::NEXT)) {
      Serial.print("(Pressed NEXT): ");

      switch (menuState) {
        case SET_HOUR:{
          time.hour = time.hour%12 + 1;
          displayConfigAlarm(menuState, time);
        }
        break;
        case SET_MINUTE: {
          time.minute = (time.minute + 5)%60;
          displayConfigAlarm(menuState, time);
        }
        break;
        case SET_AFTERNOON: {
          time.afternoon = !time.afternoon;
          displayConfigAlarm(menuState, time);
        }
        break;
        case STORE_RESULT: {
          
        }
        break;
        default:
          // You should not get here.
          break;
      }
      
    }

    if (button::pressed(button::SELECT)) {
      Serial.print("(Pressed SEL ): ");
      menuState = (menuState + 1) % 3;
      displayConfigAlarm(menuState, time);
    }

    if (button::pressed(button::MENU)) {
      Serial.print("(Pressed MENU): ");
      menuActive = false;
      int result = menuSaveResult();
      if (result) {
        if (alarmEntry == 0) {
          alarms[alarmCount] = Alarm(time.minute*60 + time.hour*3600 +12*3600*time.afternoon);
        }
      } 
    }
  }
  Serial.println("Exiting configure alarm menu");
  menu = false;
}








void writeTime(RTCTime time) {
  char time_str[8] = "00:00AM";

  int hour = time.getHour();
  int hour12 = (hour - 1) % 12 + 1;
  int minute = time.getMinutes();

  if (hour12 < 10) {
    time_str[1] += hour12;
  } else {
    time_str[0] += 1;
    time_str[1] += hour12 - 10;
  }

  if (minute < 10) {
    time_str[4] += minute;
  } else {
    time_str[3] = minute / 10 + '0';
    time_str[4] = minute % 10 + '0';
  }

  if (hour < 12) {
    time_str[5] = 'A';
  } else {
    time_str[5] = 'P';
  }

  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(1, 0, time_str);
  Serial.print(String(time_str));
}
