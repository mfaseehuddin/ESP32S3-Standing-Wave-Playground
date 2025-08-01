#include <Wire.h>
#include <Arduino.h>

void setup12() {
  Wire.begin(8, 9);  // or your SDA, SCL pins
  Serial.begin(115200);
  Serial.println("Scanning...");
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found device at 0x");
      Serial.println(addr, HEX);
    }
  }
}