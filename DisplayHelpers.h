double measurementTime(unsigned long startTime, unsigned long endTime) {
  if (startTime > 0 || endTime > 0) {
    return (endTime - startTime) / 1000.0;

  } else {
    return 0.0;
  }
};

void printSpeed(int speed) {
  display.clear();

  display.setFont(ArialMT_Plain_10);

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0,  String("0-10: ") + String(measurementTime(from0to10kmhStart, from0to10kmhEnd)));
  display.drawString(0, 10, String("0-20: ") + String(measurementTime(from0to20kmhStart, from0to20kmhEnd)));
  display.drawString(0, 20, String("0-30: ") + String(measurementTime(from0to30kmhStart, from0to30kmhEnd)));
  display.drawString(0, 30, String("0-50: ") + String(measurementTime(from0to50kmhStart, from0to50kmhEnd)));
  display.drawString(0, 40, String("0-80: ") + String(measurementTime(from0to80kmhStart, from0to80kmhEnd)));
  display.drawString(0, 50, String("0-100: ") + String(measurementTime(from0to100kmhStart, from0to100kmhEnd)));


  display.setTextAlignment(TEXT_ALIGN_RIGHT);

  display.drawString(128, 0, String(gps.satellites.value()) + String(" / ") + String(satsInView.value()) + String(" / ") + String(gps.hdop.value()));
  display.drawString(128, 38, String("km/h"));
  display.drawString(128, 50, String("MAX: ") + String(maxSpeed));

  display.setFont(ArialMT_Plain_24);
  display.drawString(128, 12, String(speed));
  display.display();
}
