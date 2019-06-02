#include <SmartDelay.h>
#include <Arduino.h>

SmartDelay::SmartDelay(unsigned long delay)
{
    this->delay = delay;
    last = millis();
}

bool SmartDelay::now()
{
    unsigned long time = millis();
    if (time - last < delay)
        return false;
    last = time;
    return true;
}
