#include "alarm.h"

Alarm::Alarm(): set(false), enabled(false), time(0), repeat(0), repeat_delay_min(0), days(0), label(NULL) {
 
}

Alarm::Alarm(time_t time, bool enabled, int repeat, int repeat_delay_min, unsigned int days, char* label):enabled(enabled), time(time), repeat(repeat), repeat_delay_min(repeat_delay_min), days(days), label(label), set(true)
{

}

void Alarm::arm(void (*callback)()) {
  // RTCTime alarmTime;
  // alarmTime.setUnixTime(this->time);
  
  // AlarmMatch matchTime;
  // matchTime.addMatchHour();
  // matchTime.addMatchMinute();

  // RTC.setAlarmCallback(callback, alarmTime, matchTime);
}


bool Alarm::isNow(time_t time_now) {
  tm_t alarmTime, currentTime = {0};

  gmtime_r(&this->time, &alarmTime);
  gmtime_r(&time_now, &currentTime);
  
  if (alarmTime.tm_hour == currentTime.tm_hour
      && alarmTime.tm_min == currentTime.tm_min
      && ((this->days == 0 && this->enabled) 
          || (this->days >> (int)currentTime.tm_wday) & 1 )) {
            return true;
          }
  return false;
}
