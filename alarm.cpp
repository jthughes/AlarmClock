#include "alarm.h"
#include "RTC.h"
Alarm::Alarm(): set(false), enabled(false), time(0), repeat(0), repeat_delay_s(0), days(0), label(NULL) {
 
}

Alarm::Alarm(time_t time, bool enabled, int repeat, int repeat_delay_s, unsigned int days, char* label):enabled(enabled), time(time), repeat(repeat), repeat_delay_s(repeat_delay_s), days(days), label(label), set(true)
{

}

void Alarm::arm(void (*callback)()) {
  RTCTime alarmTime;
  alarmTime.setUnixTime(this->time);
  
  AlarmMatch matchTime;
  matchTime.addMatchHour();
  matchTime.addMatchMinute();

  RTC.setAlarmCallback(callback, alarmTime, matchTime);
}

