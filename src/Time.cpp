#include <Time.h>

Time::Time()
{
    hour = 0;
    minute = 0;
    second = 0;
}

Time::Time(unsigned long seconds)
{
    setTotalSeconds(seconds);
}

Time::Time(int hour, int minute, int second)
{
    this->hour = hour;
    this->minute = minute;
    this->second = second;
}

void Time::setTotalSeconds(unsigned long seconds)
{
    second = seconds % 60;
    minute = (seconds / 60) % 60;
    hour = (seconds / 3600) % 24;
}

String Time::toString() const
{
    return String(hour) + ":" + String(minute) + ":" + String(second);
}
