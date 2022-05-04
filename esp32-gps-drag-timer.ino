#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
const int GPS_MODULE_ENABLE = 4;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  SerialBT.begin("ESP32-GPS"); //Bluetooth device name
  pinMode(GPS_MODULE_ENABLE, OUTPUT);
  digitalWrite(GPS_MODULE_ENABLE, HIGH);
}
int tmp;

void loop() {
  if (Serial2.available()) {
    tmp = Serial2.read();
    SerialBT.write(tmp);
    Serial.write(tmp);
  }
  if (SerialBT.available()) {
    Serial2.write(SerialBT.read());
  }
}
