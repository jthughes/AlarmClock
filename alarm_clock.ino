#include "arduino_secrets.h"

#include "RTC.h"



// OLED
#include <U8x8lib.h>
U8X8_SSD1306_128X64_NONAME_4W_SW_SPI u8x8(/* clock=*/ 12, /* data=*/ 11, /* cs=*/ 8, /* dc=*/ 9, /* reset=*/ 10);



bool tick;


void setup() {
  Serial.begin(9600);
  while (!Serial);

  get_network_time();

  tick = true;
  // RTC.begin();
  RTC.setPeriodicCallback(clockTick, Period::ONCE_EVERY_1_SEC);


  u8x8.begin();

}

void loop() {

  if (tick) {
    tick = false;
    RTCTime currentTime;
    RTC.getTime(currentTime);
    if (currentTime.getSeconds() == 0) {
      int hour = currentTime.getHour();
      int hour12 = (hour - 1) % 12 + 1;
      int minute = currentTime.getMinutes();
      char time[8];
      if (hour12 < 10) {
        time[0] = '0';
        time[1] = hour12 + '0';
      } else {
        time[0] = '1';
        time[1] = hour12 + '0' - 10;
      }
      time[2] = ':';
      if (minute < 10) {
        time[3] = '0';
        time[4] = minute + '0';
      } else {
        time[3] = minute / 10 + '0';
        time[4] = minute % 10 + '0';
      }
      if (hour < 12) {
        time[5] = 'A';
      } else {
        time[5] = 'P';
      }
      time[6] = 'M';
      time[7] = 0;

      // drawText(time);
      u8x8.setFont(u8x8_font_chroma48medium8_r);
      u8x8.drawString(1, 0, time);
      Serial.println("Time is "+ String(currentTime));
       
    }
  }
}

void clockTick() {
  tick = true;
}



