#include "Wire.h"
#include "SSD1306Wire.h"
#include "TinyGPS++.h"
#include "math.h"
#include "ArduinoJson.h"
#include "SPIFFS.h"

TinyGPSPlus gps;
TinyGPSCustom satsInView(gps, "GPGSV", 3);
SSD1306Wire display(0x3c, 5, 4);
DynamicJsonDocument doc(1024);
static File SpiffsFile;
char charBuf[100];

//#define DEVELOPMENT_MODE

#include "speedHelpers.h"
#include "DisplayHelpers.h"
#include "GpsModuleHelpers.h"

const int GPS_MODULE_ENABLE = 21;

void setup() {
  pinMode(GPS_MODULE_ENABLE, OUTPUT);
  digitalWrite(GPS_MODULE_ENABLE, HIGH);

  Serial.begin(115200); // USB Serial

  Serial.println("BUILD_TIMESTAMP " + String(__DATE__) + " " + String(__TIME__)); 
  Serial.println("FREESPIFFS " + (SPIFFS.totalBytes() - SPIFFS.usedBytes()));
  Serial.println("USEDSPIFFS " + SPIFFS.usedBytes());
  Serial.println("TOTALSPIFFS " + SPIFFS.totalBytes());


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
        if(from0to10kmhStart != 0){
          doc["date"] = dateFromGPS();
          doc["0-10"] = String(measurementTime(from0to10kmhStart, from0to10kmhEnd));
          doc["0-20"] = String(measurementTime(from0to20kmhStart, from0to20kmhEnd));
          doc["0-30"] = String(measurementTime(from0to30kmhStart, from0to30kmhEnd));
          doc["0-50"] = String(measurementTime(from0to50kmhStart, from0to50kmhEnd));
          doc["0-80"] = String(measurementTime(from0to80kmhStart, from0to80kmhEnd));
          doc["0-100"] = String(measurementTime(from0to100kmhStart, from0to100kmhEnd));


          File file = SPIFFS.open(("/" + dateFromGPS() + ".json"), "w");
          serializeJson(doc, file);
          file.close();
                    
          from0to10kmhStart = 0;
          from0to20kmhStart = 0;
          from0to30kmhStart = 0;
          from0to50kmhStart = 0;
          from0to80kmhStart = 0;
          from0to100kmhStart = 0;
        }
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
