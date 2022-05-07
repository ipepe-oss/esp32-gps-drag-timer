#include "Wire.h"
#include "SSD1306Wire.h"
#include "TinyGPS++.h"
#include "math.h"

TinyGPSPlus gps;
SSD1306Wire  display(0x3c, 5, 4);

#include "DisplayHelpers.h"
#include "GpsModuleHelpers.h"

const int GPS_MODULE_ENABLE = 21;

void setup() {
  pinMode(GPS_MODULE_ENABLE, OUTPUT);
  digitalWrite(GPS_MODULE_ENABLE, HIGH);
  
  Serial.begin(115200); // USB Serial

  // Setup GPS Serial to 10hz and 115200
  setupGpsModule();
  Serial2.begin(115200);

  // Setup OLED Display
  display.init();
  display.setContrast(255);
}

void loop() {
  if (Serial2.available()) {
    gps.encode(Serial2.read());
  }
  if (gps.speed.isUpdated()){
    printSpeed(ceil(gps.speed.kmph()));
  }
}
