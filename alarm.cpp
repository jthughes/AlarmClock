#include "alarm.h"

Alarm::Alarm(): set(false), enabled(false), time(0), repeat(0), repeat_delay_min(5), days(0), label(NULL) {
 
}

Alarm::Alarm(time_t time, bool enabled, int repeat, int repeat_delay_min, unsigned int days, char* label):enabled(enabled), time(time), repeat(repeat), repeat_delay_min(repeat_delay_min), days(days), label(label), set(true)
{

}

bool match_hour_minute(tm_t* first, tm_t* second) {
  if (!first || !second) {
    return false;
  }
  return first->tm_hour == second->tm_hour && first->tm_min == second->tm_min;
}

void Alarm::setDay(day_t day) {
  this->days = this->days | (1 << day);
}

void Alarm::unsetDay(day_t day) {
  this->days = this->days & ~(1 << day);
}

bool Alarm::isDaySet(day_t day) {
  return (this->days & (1 << day)) != 0;
}

void Alarm::toggleDay(day_t day) {
  this->days = this->days ^ (1 << day);
}

unsigned int Alarm::getDays() {
  return this->days;
};

bool Alarm::isNow(time_t time_now) {
  if (this->enabled == false) {
    return false;
  }

  tm_t alarmTime, currentTime = {0};

  gmtime_r(&time_now, &currentTime);
  
  // Create an array of any recurring alarms;
  const int alarm_count = this->repeat + 1;
  time_t alarm_time[alarm_count] = {0};
  alarm_time[0] = this->time;
  for (int i = 1; i <= this->repeat; i += 1) {
    alarm_time[i] = alarm_time[i-1] + 60*this->repeat_delay_min;
  }
  
  // Check each alarm
  for (int i = 0; i < alarm_count; i += 1) {
    gmtime_r(&alarm_time[i], &alarmTime);
    
    // If either any day, or today is one of the set days, return true if current time is now
    if ((this->days == 0 || this->isDaySet((day_t)currentTime.tm_wday)) && match_hour_minute(&alarmTime, &currentTime)) {
      return true;
    }
  }  
  return false;
}
