#include "config.h"
#include <TinyGPS++.h>
#include <movingAvgFloat.h>
TinyGPSPlus gps;

struct Data {
  float homeLatitude;       // Save home Lat
  float homeLongitude;      // Save home Long
  float homeAltitude;       // Save home gps altitude
  float homePressure;       // Baro calibration
};
struct Data settings;

static uint32_t tLoop = 0;
float vSpeed = 0.0;             // Vertical Speed
float baroAlt = 0;              // Barometric altitude from BMP180

void setup() {
#ifdef DEVMODE
  Serial.begin(115200);
#endif
  setupBaro();
  setupGps();
}

void loop() {

  updateVerticalSpeed();
  updateBaroAlt();
  updateGps();

  if (setGpsHome() == true) {
    if (setBaroSeaLevel() == true) {
      writeSettings();
      // EasyBuzzer.singleBeep(2000, 500);                     // Beep when the calibrated
    }
  }
  if (millis() - tLoop >= 500) {
    tLoop = millis();
    Serial.print("Baro: ");
    Serial.print(baroAlt);
    Serial.print(" vS: ");
    Serial.print(vSpeed);
    Serial.print(" Lat: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(" Lon: ");
    Serial.print(gps.location.lng(), 6);
    Serial.print(" Alt: ");
    Serial.print(gps.altitude.meters());
    Serial.print(" Sat: ");
    Serial.print(gps.satellites.value());
    Serial.print(" Age: ");
    Serial.print(gps.location.age());
    Serial.print(" HDOP: ");
    Serial.println(gps.hdop.value());
  }
}
