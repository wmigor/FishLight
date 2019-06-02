#include <WiFiConnector.h>
#include <ESP8266WiFi.h>

WiFiConnector::WiFiConnector(const String &ssid, const String &password)
{
    this->ssid = ssid;
    this->password = password;
}

void WiFiConnector::connect()
{
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
}

void WiFiConnector::disconnect()
{
    WiFi.disconnect(true);
}

bool WiFiConnector::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}