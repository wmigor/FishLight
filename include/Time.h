#ifndef Time_h
#define Time_h

#include <Arduino.h>

class Time
{
public:
    Time();
    Time(int hour, int minute, int second);
    Time(unsigned long seconds);
    inline int getSecond() const { return second; }
    inline int getMinute() const { return minute; }
    inline int getHour() const { return hour; }
    inline void setHour(int hour) { this->hour = hour; }
    inline void setMinute(int minute) { this->minute = minute; }
    inline void setSecond(int second) { this->second = second; }
    inline int getTotalSeconds() const { return second + minute * 60 + hour * 3600; }
    void setTotalSeconds(unsigned long seconds);
    String toString() const;

private:
    int hour;
    int minute;
    int second;
};

#endif
