void printSpeed(int speed) {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_24);
  display.drawString(64, 40, (String(speed) + String(" km/h")));
  display.display();
}