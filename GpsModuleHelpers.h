// GPS Module pinout
// P16 - blue
// P17 - green
// P21 - yellow
// 3.3V/VCC - red
// GND - black

const unsigned char ubxRate10Hz[] PROGMEM = { 0x06, 0x08, 0x06, 0x00, 100, 0x00, 0x01, 0x00, 0x01, 0x00 };

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

void setupGpsModule() {
  Serial2.begin(9600);
  delay(1000);
  sendUBX( ubxRate10Hz, sizeof(ubxRate10Hz) );
  Serial2.print("$PUBX,41,1,0003,0003,115200,0*1C\r\n"); // change baud rate to 115200
  Serial2.flush();
  Serial2.end();
}
