#include <NetworkIdentify.h>

NetworkIdentify::NetworkIdentify(const String &name, const String &type, int port) : udp()
{
  this->name = name;
  this->type = type;
  this->port = port;
  sendText = "{\"name\": \"" + name + "\", \"type\": \"" + type + "\"}";
}

void NetworkIdentify::setup()
{
  udp.begin(port);
}

void NetworkIdentify::handle()
{
  int packetSize = udp.parsePacket();
  if (packetSize)
  {
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, udp.remoteIP().toString().c_str(), udp.remotePort());
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.write(sendText.c_str());
    udp.endPacket();
    Serial.printf("UDP packet send: %s\n", sendText.c_str());
  }
}
