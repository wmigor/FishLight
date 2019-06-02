#ifndef NetworkIdentify_h
#define NetworkIdentify_h

#include <WiFiUdp.h>
#include <Arduino.h>

class NetworkIdentify
{
public:
    NetworkIdentify(const String &name, const String &type, int port);

    void setup();
    void handle();

private:
    String name;
    String type;
    String sendText;
    int port;
    WiFiUDP udp;
};

#endif

