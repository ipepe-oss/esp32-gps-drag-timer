#include <Wire.h>  
#include "SSD1306Wire.h"
#include "TinyGPS++.h"
TinyGPSPlus gps;


#define SDA 5
#define SCL 4
SSD1306Wire  display(0x3c, SDA, SCL);


//gps pinout
// P16 - blue
// P17 - green
// P21 - yellow
// 3.3V/VCC - red
// GND - black

void printSpeed(void) {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_24);
  display.drawString(64, 40, String(gps.speed.kmph() + String(" km/h")));
  display.display();
}

const int GPS_MODULE_ENABLE = 21;

const unsigned char ubxRate10Hz[] PROGMEM =
  { 0x06,0x08,0x06,0x00,100,0x00,0x01,0x00,0x01,0x00 };

void sendUBX( const unsigned char *progmemBytes, size_t len )
{
  Serial2.write( 0xB5 ); // SYNC1
  Serial2.write( 0x62 ); // SYNC2

  uint8_t a = 0, b = 0;
  while (len-- > 0) {
    uint8_t c = pgm_read_byte( progmemBytes++ );
    a += c;
    b += a;
    Serial2.write( c );
  }

  Serial2.write( a ); // CHECKSUM A
  Serial2.write( b ); // CHECKSUM B

}

void setup() {
  pinMode(GPS_MODULE_ENABLE, OUTPUT);
  digitalWrite(GPS_MODULE_ENABLE, HIGH);
  
  Serial.begin(115200); // USB Serial

  // Setup GPS Serial to 10hz and 115200
  
  Serial2.begin(9600);
  delay(1000);
  sendUBX( ubxRate10Hz, sizeof(ubxRate10Hz) );
  Serial2.print("$PUBX,41,1,0003,0003,115200,0*1C\r\n"); // change baud rate to 115200
  Serial2.flush();
  Serial2.end();
  Serial2.begin(115200);

  // Setup OLED Display

  display.init();
  display.setContrast(255);

}
int tmp;

void loop() {
  if (Serial2.available()) {
    tmp = Serial2.read();
//    Serial.write(tmp);
    gps.encode(tmp);
  }
  if (gps.speed.isUpdated()){
    Serial.println(gps.speed.kmph());
    printSpeed();
  }
}
