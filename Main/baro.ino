#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp(&Wire);
movingAvg baroAltAvg(10);    // use 10 data points for the moving average

// Barometer
float baroSeaLevel = 1013;
unsigned long tBaroStab = 0;
static uint32_t tBaro = 0;
static uint32_t timeStart = 0;
static uint32_t timeEnd = 0;
float baroAlt = 0;              // Barometric altitude from BMP180
float prevBaroAlt = 0;          //
float baroTemp = 0;             // Barometric temperature from BMP180
float dt = 0;
bool nowBaroStab = false;       // Flag for vertical speed stable

void setupBaro() {

  baroAltAvg.begin();
  delay(5);                 // Wait for BMP180 to power up
  bmp.begin(BARO_ADDRESS);
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,       // Operating Mode
                  Adafruit_BMP280::SAMPLING_X1,       // Temp Oversampling
                  Adafruit_BMP280::SAMPLING_X2,       // Pressure Oversampling
                  Adafruit_BMP280::FILTER_X16,        // Filtering
                  Adafruit_BMP280::STANDBY_MS_1);     // Standby time  1, 63, 125, 250, 500, 1000, 2000, 4000
#ifdef DEVMODE
  Serial.println("Barometer setup complete");
#endif
}

//--------------------------------------------- Update Sensor ------------------------------------------------//
void computeBaroAlt(int t) {
  if ((millis() - tBaro) >= t) {
    tBaro = millis();
    baroTemp = bmp.readTemperature();                       // Read temperature
    baroAlt = bmp.readAltitude(baroSeaLevel) * 100;               // Read sensor
    baroAlt = baroAltAvg.reading(baroAlt);                  // Commit new reading to moving average array and read the average
    computeVerticalSpeed();                                 // Update the vertical speed using the new reading
  }
}

void resetBaroAvg() {
  //  baroAltAvg.reset();
}

//------------------------------------- Calibrate GPS altitude to baro----------------------------------------//

bool setBaroSeaLevel() {
  if (abs(baroAlt - gps.altitude.meters()) > 0.01) {
    baroSeaLevel = (baroSeaLevel + ((gps.altitude.meters() - baroAlt) / 8));      // Update baro sea level untill GPS and Baro are the same
    return false;
  }
  settings.homePressure = baroSeaLevel;                 // Baro calibration
  EasyBuzzer.singleBeep(2000, 500);                     // Beep when the calibrated
#ifdef DEVMODE
  Serial.println("Barometer Mean Sea Level calibrated");
#endif
  return true;
}

// ------------------------------------------ Vertical Speed Stable ------------------------------------------ //
bool baroStab() {
  if (fabs(vSpeed) > 0.2  || nowBaroStab == false) {
    tBaroStab = millis();                                              // Reset timer
  }
  if (millis() - tBaroStab > 1000) {                                   // If timer hasnt reset within 1 second altitude is stable
    nowBaroStab = false;                                               // Reset started flag
    return true;
  }
  else {
    nowBaroStab = true;
    return false;
  }
}

// ------------------------------------------ Update  Vertical Speed ------------------------------------------- //
float vSpeed() {
  float vSpeed = 0;            // make a function to remove
  timeStart = millis();
  float dt = timeStart - timeEnd;
  if (dt >= 1000) {
    float distance = baroAlt - prevBaroAlt;
    vSpeed = (distance / ((float)dt / 1000.0));
    prevBaroAlt = baroAlt;
    timeEnd = timeStart;
  }
}
