#ifndef ALARM_H
#define ALARM_H

#include <time.h>

typedef struct tm tm_t;

typedef enum day {
  SUN,
  MON,
  TUE,
  WED,
  THU,
  FRI,
  SAT
} day_t;

class Alarm {
  public:
    Alarm();
    Alarm(time_t time, bool enabled = true, int repeat = 0, int repeat_delay_min = 5, unsigned int days = 0, char* label = NULL);
    
    bool isNow(time_t time_now);

    void setDay(day_t day);
    void unsetDay(day_t day);
    void toggleDay(day_t day);
    bool isDaySet(day_t day);
    unsigned int getDays();

    static const int     repeatMax = 5;

    // Used to check if space in alarm array is empty or not.
    bool          set;
    // Used to check if an alarm will go off.
    bool          enabled;
    
    char          *label;
    time_t        time;
    int           repeat;
    int           repeat_delay_min;

  private:
    unsigned int  days;
    //music track
};

#endif // ALARM_H