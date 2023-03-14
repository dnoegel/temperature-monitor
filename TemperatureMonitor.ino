#include "Setup.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include <OneWire.h>
#include <DallasTemperature.h>

WiFiClient espClient;
PubSubClient client(espClient);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


String ip;
char charBuf[40];

void setup()
{
  Serial.begin(SERIAL_SPEED);

  ensureWifiConnected();
  setupWebUpdater();
  setupMqtt();

}

void loop()
{
  char tempStr[10];
  
  ensureWifiConnected();
  loopMqtt();
  loopWebUpdater();

  sensors.requestTemperatures();

  // Sensor 1
  float temp1 = sensors.getTempCByIndex(0);
  Serial.print("Temperature 1: "); Serial.print(temp1);  Serial.println(" °C");
  dtostrf(temp1, 2, 2, tempStr); 
  client.publish("mqtt.0.tempmon.temp1", tempStr);

  // Sensor 2
  float temp2 = sensors.getTempCByIndex(1);
  Serial.print("Temperature 2: "); Serial.print(temp2); Serial.println(" °C");  
  dtostrf(temp2, 2, 2, tempStr); 
  client.publish("mqtt.0.tempmon.temp2", tempStr);


  delay(5000);

}
