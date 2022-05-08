void printSpeed(int speed) {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, String(gps.satellites.value()) + String(" / ") + String(satsInView.value()) + String(" / ") + String(gps.hdop.value()));
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 40, (String(speed) + String(" km/h")));
  display.display();
}
