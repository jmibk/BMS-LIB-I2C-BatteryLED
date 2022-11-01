#include <Arduino.h>
#include "i2c_batteryled.h"

BatteryLED batteryLED = BatteryLED();
TwoWire WIREBUS = TwoWire(1);

void setup() {
  //init serial connection
  Serial.begin(115200);
  Serial.println("Demo - Battery LED State over I2C");
 
  //init i2c
  WIREBUS.begin(4, 17, 1000ul);
  batteryLED.setBus(WIREBUS);
  /*
  if (batteryLED.init(4, 17))  //SDA, SCL
    Serial.println("LED module initialised");
  else
    Serial.println("LED module failed!");
    */
  }

void loop() {
  Serial.println("Send Data...");
  batteryLED.setSOCenable(true);
  batteryLED.setSOC((millis()/1000)%100);
  batteryLED.setMaxIntensity(80);
  batteryLED.setChargingState(OFF);
  batteryLED.compute();
  Serial.println(batteryLED.getCommunicationErrorState());
  Serial.println("");
  delay(200);
  }