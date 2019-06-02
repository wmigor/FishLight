#ifndef WiFiConnector_h
#define WiFiConnector_h

#include <Arduino.h>

class WiFiConnector
{
public:
    WiFiConnector(const String &ssid, const String &password);
    void connect();
    void disconnect();
    bool isConnected();
private:
    String ssid;
    String password;
};

#endif

