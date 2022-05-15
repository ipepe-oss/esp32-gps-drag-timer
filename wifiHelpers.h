#ifdef DEVELOPMENT_MODE
#include "devWifiCredentials.h"
#endif

const char* host = "esp32-gps";

void setupWifi() {
#ifdef DEVELOPMENT_MODE
  // connect to existing WiFi access point as a station

  Serial.print("\r\nConnecting to existing Wifi Access Point : ");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
  }
  Serial.print("Wifi Connected: "); Serial.println(WiFi.localIP());
#else // set up as stand-alone WiFi Access Point
  WiFi.mode(WIFI_AP);
  bool result =  WiFi.softAP(host, host);
  Serial.println(result == true ? "AP setup OK" : "AP setup failed");
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(myIP);

#endif
}
