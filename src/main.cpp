#include <Arduino.h>
#include "i2c_batteryled.h"

BatteryLED batteryLED = BatteryLED();
TwoWire WIREBUS = TwoWire(1);

void setup() {
  //init serial connection
  Serial.begin(115200);
  Serial.println("Demo - Battery LED State over I2C");
 
  //init i2c
  batteryLED.setBus(WIREBUS);
  if (batteryLED.init(4, 17))  //SDA, SCL
    Serial.println("LED module initialised");
  else
    Serial.println("LED module failed!");
  }

void loop() {
  Serial.println("Send Data...");
  batteryLED.setSOCenable(true);
  batteryLED.setSOC((millis()/1000)%100);
  batteryLED.compute();

  Serial.println("");
  delay(200);
  }