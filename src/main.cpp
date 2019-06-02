#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiConnector.h>
#include <NetworkIdentify.h>
#include <ArduinoJson.h>
#include <Time.h>
#include <SmartDelay.h>

NetworkIdentify networkIdentify("FishLight", "FishLight", 54545);
WiFiConnector connector("ssid", "password");
ESP8266WebServer server(80);
SmartDelay timeRequestDelay(1000 * 3600);
SmartDelay checkRelayDelay(5000ul);
SmartDelay updateCurrentTimeDelay(1000ul);
Time startTime(8, 0, 0);
Time finishTime(20, 0, 0);
Time currentTime(startTime);
Time lastRequestedTime(currentTime);

const unsigned long timeZoneSeconds = 3 * 3600;
unsigned long lastMillis;
bool timeSinchronized = false;

bool isEnabled()
{
  int start = startTime.getTotalSeconds();
  int finish = finishTime.getTotalSeconds();
  int current = currentTime.getTotalSeconds();
  if (start < finish)
    return current >= start && current <= finish;
  return current <= start || current >= finish;
}

void serverHandleRoot()
{
  String message = "Start time: " + startTime.toString();
  message += "<br>Finish time: " + finishTime.toString();
  message += "<br>Current time: " + currentTime.toString();
  message += "<br>Last requested time: " + lastRequestedTime.toString();
  message += "<br>Sinchronized: " + String(timeSinchronized ? "true": "false");
  message += "<br>Enabled: " + String(isEnabled() ? "true": "false");

  server.send(200, "text/html", message);
}

void serverHandleConfig()
{
  String strStartHour = server.arg("start_hour");
  String strStartMinute = server.arg("start_minute");
  String strWorkMinutes = server.arg("work_minutes");

  if (strStartHour ==  "" || strStartMinute == "" || strWorkMinutes == "")
  {
    int work_seconds = finishTime.getTotalSeconds() - startTime.getTotalSeconds();
    String message = "{";
    message += "\"start_hour\": " + String(startTime.getHour()) + ",";
    message += "\"start_minute\": " + String(startTime.getMinute()) + ",";
    message += "\"work_minutes\": " + String(work_seconds / 60) + ",";

    message += "\"current_time\": \"" + currentTime.toString() + "\",";
    message += "\"last_requested_time\": \"" + lastRequestedTime.toString() + "\",";
    message += "\"sinchronized\": " + String(timeSinchronized ? "true": "false") + ", ";
    message += "\"enabled\": " + String(isEnabled() ? "true": "false");

    message += "}";
    server.send(200, "application/json", message);
    return;
  }
  startTime.setHour(strStartHour.toInt());
  startTime.setMinute(strStartMinute.toInt());
  finishTime.setTotalSeconds(startTime.getTotalSeconds() + strWorkMinutes.toInt() * 60);
  server.send(200, "text/html", "OK");
}

void serverHandleNotFound()
{
  server.send(404, "text/plain", "Not found");
}

void serverStart()
{
  server.on("/", serverHandleRoot);
  server.on("/config", serverHandleConfig);
  server.onNotFound(serverHandleNotFound);
  server.begin();
}

bool requestTime()
{
  Serial.print(WiFi.localIP());
  const String url = "http://worldtimeapi.org/api/timezone/Europe/Moscow";
  HTTPClient http;

  http.begin(url);
  int httpCode = http.GET();
  bool result = false;

  if (httpCode > 0)
  {
    String response = http.getString();
    StaticJsonDocument<1024> doc;
    DeserializationError error = ArduinoJson::deserializeJson(doc, response);
    if (!error)
    {
      unsigned long totalSeconds = doc["unixtime"].as<unsigned long>();
      lastRequestedTime.setTotalSeconds(totalSeconds + timeZoneSeconds);
      lastMillis = millis();
      result = true;
    }
  }

  http.end();
  return result;
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(0, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  connector.connect();
  serverStart();
  networkIdentify.setup();
  timeSinchronized = false;
}

void loop()
{
  networkIdentify.handle();
  server.handleClient();
  
  if (updateCurrentTimeDelay.now())
  {
    unsigned long delta = millis() - lastMillis;
    currentTime.setTotalSeconds(lastRequestedTime.getTotalSeconds() + delta / 1000);
  }

  if (timeRequestDelay.now())
    timeSinchronized = false;

  if (connector.isConnected() && !timeSinchronized)
    timeSinchronized = requestTime();

  if (checkRelayDelay.now())
  {
    bool enabled = isEnabled();

    digitalWrite(LED_BUILTIN, enabled ? LOW : HIGH);
    digitalWrite(0, enabled ? LOW : HIGH);
  }
  delay(100);
}
