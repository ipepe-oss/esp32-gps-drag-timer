#include "Wire.h"
#include "SSD1306Wire.h"
#include "TinyGPS++.h"
#include "math.h"
#include "Vector.h"

TinyGPSPlus gps;
TinyGPSCustom satsInView(gps, "GPGSV", 3);
SSD1306Wire  display(0x3c, 5, 4);

#define DEVELOPMENT_MODE

unsigned int maxSpeed = 0;
int tmpCurrentSpeed;

unsigned long currentMeasurementStart = 0;
unsigned long from0to10kmhStart = 0;
unsigned long from0to10kmhEnd = 0;
unsigned long from0to20kmhStart = 0;
unsigned long from0to20kmhEnd = 0;
unsigned long from0to30kmhStart = 0;
unsigned long from0to30kmhEnd = 0;
unsigned long from0to50kmhStart = 0;
unsigned long from0to50kmhEnd = 0;
unsigned long from0to80kmhStart = 0;
unsigned long from0to80kmhEnd = 0;
unsigned long from0to100kmhStart = 0;
unsigned long from0to100kmhEnd = 0;

int currentSpeed() {
  #ifdef DEVELOPMENT_MODE
    return ceil(millis() / 1000.0) - 1;
  #else
    return floor(gps.speed.kmph());
  #endif
}
bool updateScreen(){
  #ifdef DEVELOPMENT_MODE
    return true;
  #else
    return gps.speed.isUpdated();
  #endif
}

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
  printSpeed(0);

  Serial.printf("\r\n\r\nBinary compiled on %s at %s\r\n", __DATE__, __TIME__);
}

void loop() {
  if (Serial2.available()) {
    gps.encode(Serial2.read());
  }
  if (updateScreen()) {
    tmpCurrentSpeed = currentSpeed();
    printSpeed(tmpCurrentSpeed);
    if (tmpCurrentSpeed > maxSpeed) {
      maxSpeed = tmpCurrentSpeed;
    }
    if (tmpCurrentSpeed >= 1) {
      if (currentMeasurementStart == 0) {
        currentMeasurementStart = millis();
      } else {
        if (from0to10kmhStart == 0 && tmpCurrentSpeed > 10) {
          from0to10kmhStart = currentMeasurementStart;
          from0to10kmhEnd = millis();
        }
        if (from0to20kmhStart == 0 && tmpCurrentSpeed > 20) {
          from0to20kmhStart = currentMeasurementStart;
          from0to20kmhEnd = millis();
        }
        if (from0to30kmhStart == 0 && tmpCurrentSpeed > 30) {
          from0to30kmhStart = currentMeasurementStart;
          from0to30kmhEnd = millis();
        }
        if (from0to50kmhStart == 0 && tmpCurrentSpeed > 50) {
          from0to50kmhStart = currentMeasurementStart;
          from0to50kmhEnd = millis();
        }
        if (from0to80kmhStart == 0 && tmpCurrentSpeed > 80) {
          from0to80kmhStart = currentMeasurementStart;
          from0to80kmhEnd = millis();
        }
        if (from0to100kmhStart == 0 && tmpCurrentSpeed > 100) {
          from0to100kmhStart = currentMeasurementStart;
          from0to100kmhEnd = millis();
        }
      }
    } else if (tmpCurrentSpeed == 0) {
      currentMeasurementStart = 0;
    }
  }
}
