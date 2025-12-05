#include "alarm.h"
#include "RTC.h"
Alarm::Alarm(): set(false), enabled(false), time(0), repeat(0), repeat_delay_min(0), days(0), label(NULL) {
 
}

Alarm::Alarm(time_t time, bool enabled, int repeat, int repeat_delay_min, unsigned int days, char* label):enabled(enabled), time(time), repeat(repeat), repeat_delay_min(repeat_delay_min), days(days), label(label), set(true)
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


bool Alarm::isNow() {
  RTCTime alarmTime;
  alarmTime.setUnixTime(this->time);

  RTCTime currentTime;
  RTC.getTime(currentTime);
  
  if (alarmTime.getHour() == currentTime.getHour()
      && alarmTime.getMinutes() == currentTime.getMinutes()
      && ((this->days == 0 && this->enabled) 
          || (this->days >> (int)currentTime.getDayOfWeek()) & 1 )) {
            return true;
          }
  return false;
}
