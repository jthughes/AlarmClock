#ifndef ALARM_H
#define ALARM_H

#include <time.h>

class Alarm {
  public:
    Alarm();
    Alarm(time_t time, bool enabled = true, int repeat = 0, int repeat_delay_s = 300, unsigned int days = 0, char* label = NULL);
    
    void arm(void (*callback)());

  private:
    bool          set;
    bool          enabled;
    char          *label;
    time_t        time;
    int           repeat;
    int           repeat_delay_s;
    unsigned int  days;
    //music track
};

#endif // ALARM_H