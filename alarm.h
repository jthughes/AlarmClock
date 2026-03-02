#ifndef ALARM_H
#define ALARM_H

#include <time.h>

typedef struct tm tm_t;

class Alarm {
  public:
    Alarm();
    Alarm(time_t time, bool enabled = true, int repeat = 0, int repeat_delay_min = 30, unsigned int days = 0, char* label = NULL);
    
    void arm(void (*callback)());

    bool isNow(time_t time_now);

    bool          set;
    bool          enabled;
    char          *label;
    time_t        time;
    int           repeat;
    int           repeat_delay_min;
    unsigned int  days;

    static const int     repeatMax = 5;

  private:

    //music track
};

#endif // ALARM_H