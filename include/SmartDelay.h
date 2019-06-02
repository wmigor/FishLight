#ifndef SmartDelay_h
#define SmartDelay_h

class SmartDelay
{
public:
    SmartDelay(unsigned long delay);

    bool now();
private:
    unsigned long delay;
    unsigned long last;
};

#endif

