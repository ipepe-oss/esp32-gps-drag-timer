#include "Wire.h"
#include "SSD1306Wire.h"
#include "TinyGPS++.h"
#include "math.h"
#include "SPIFFS.h"
#include "WiFi.h"
#include "ESPAsyncWebServer.h"

TaskHandle_t Cpu2Task;
TinyGPSPlus gps;
TinyGPSCustom satsInView(gps, "GPGSV", 3);
SSD1306Wire display(0x3c, 5, 4);
static File SpiffsFile;
AsyncWebServer *server = NULL;

//#define DEVELOPMENT_MODE

#include "wifiHelpers.h"
#include "webServerHelper.h"
#include "speedHelpers.h"
#include "DisplayHelpers.h"
#include "GpsModuleHelpers.h"

const int GPS_MODULE_ENABLE = 21;

void setup() {
  delay(500);

  Serial.begin(115200); // USB Serial
  delay(500);
  Serial.println("[setup] Serial established");

  // Setup GPS Serial to 10hz and 115200
  pinMode(GPS_MODULE_ENABLE, OUTPUT);
  digitalWrite(GPS_MODULE_ENABLE, HIGH);
  setupGpsModule();
  Serial2.begin(115200);
  Serial.println("[setup] Serial2 established");

  // Setup OLED Display
  display.init();
  display.setContrast(255);
  printSpeed(0);
  Serial.println("[setup] Display initiated");

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  Serial.println("BUILD_TIMESTAMP " + String(__DATE__) + " " + String(__TIME__));
  Serial.print("USEDSPIFFS: ");
  Serial.println(SPIFFS.usedBytes());
  Serial.print("TOTALSPIFFS: ");
  Serial.println(SPIFFS.totalBytes());

  setupWifi();
  setupWebserver();
  Serial.println("[setup] done");

}

void saveMeasurementToSpiffs() {
  String measurementPage = "";
  measurementPage += "<center><table>";
  measurementPage += "<tr><td>Datetime:</td><td>" + dateFromGPS() + "</td></tr>";
  measurementPage += String("<tr><td>Max:</td><td>" + String(maxSpeed) + "</td></tr>");
  measurementPage += String("<tr><td>0-10:</td><td>" + String(measurementTime(from0to10kmhStart, from0to10kmhEnd)) + "</td></tr>");
  measurementPage += String("<tr><td>0-20:</td><td>" + String(measurementTime(from0to20kmhStart, from0to20kmhEnd)) + "</td></tr>");
  measurementPage += String("<tr><td>0-30:</td><td>" + String(measurementTime(from0to30kmhStart, from0to30kmhEnd)) + "</td></tr>");
  measurementPage += String("<tr><td>0-50:</td><td>" + String(measurementTime(from0to50kmhStart, from0to50kmhEnd)) + "</td></tr>");
  measurementPage += String("<tr><td>0-80:</td><td>" + String(measurementTime(from0to80kmhStart, from0to80kmhEnd)) + "</td></tr>");
  measurementPage += String("<tr><td>0-100:</td><td>" + String(measurementTime(from0to100kmhStart, from0to100kmhEnd)));
  measurementPage += "</table></center>";

  File file = SPIFFS.open(("/" + String(millis()) + ".html"), "w");
  file.print(measurementPage);
  file.close();
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
        if (from0to10kmhEnd != 0) {
          from0to10kmhStart = 0;
          from0to20kmhStart = 0;
          from0to30kmhStart = 0;
          from0to50kmhStart = 0;
          from0to80kmhStart = 0;
          from0to100kmhStart = 0;
          from0to10kmhEnd = 0;
          from0to20kmhEnd = 0;
          from0to30kmhEnd = 0;
          from0to50kmhEnd = 0;
          from0to80kmhEnd = 0;
          from0to100kmhEnd = 0;
          maxSpeed = 0;
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
