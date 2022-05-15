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
