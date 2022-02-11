#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp(&Wire);
movingAvgFloat baroAltAvg(20);    // use 10 data points for the moving average
movingAvgFloat vSpeedAvg(20);    // use 10 data points for the moving average

// Barometer
float baroSeaLevel = 1015.00;   // Starting mBar 
static uint32_t tStab = 0;      // Barometer stable timer
static uint32_t tBaro = 0;      // Barometer update timer
static uint32_t tSpeed = 0;     // Vertical speed timer
bool bStab = false;             // Flag for vertical speed stable
bool bCal = false;              // Flag for mean sea level calibrated
//float baroAlt = 0;              // Barometric altitude from BMP180
float prevBaroAlt = 0;          // Previous altitude for vertical speed calc
float baroTemp = 0;             // Barometric temperature from BMP180
// vSpeed = 0.0;             // Vertical Speed

void setupBaro() {
  
  baroAltAvg.begin();
  vSpeedAvg.begin();

  delay(10);                 // Wait for BMP180 to power up
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

bool updateBaroAlt() {
  if (millis() - tBaro >= BARO_UPDATE) {
    tBaro = millis();
    baroTemp = bmp.readTemperature();                       // Read temperature
    baroAlt = bmp.readAltitude(baroSeaLevel);               // Read sensor
    baroAlt = baroAltAvg.reading(baroAlt);                  // Commit new reading to moving average array and read the average
    //computeVerticalSpeed();                                 // Update the vertical speed using the new reading
    return true;
  }
  return false;
}

//------------------------------------- Calibrate GPS altitude to baro----------------------------------------//

bool setBaroSeaLevel() {
  if (bCal == false && updateBaroAlt() == true) {
    if (fabs(baroAlt - gps.altitude.meters()) > 0.01) {
      baroSeaLevel = (baroSeaLevel + ((gps.altitude.meters() - baroAlt) / 100));      // Update baro sea level untill GPS and Baro are the same
      return false;
    }
    else {
       settings.homePressure = baroSeaLevel;                 // Baro calibration
      // EasyBuzzer.singleBeep(2000, 500);                     // Beep when the calibrated
#ifdef DEVMODE
      Serial.print("Barometer Mean Sea Level calibrated at: ");
      Serial.println(baroSeaLevel);
      Serial.print("Barometer Altitude: ");
      Serial.println(baroAlt);
#endif
      bCal = true;
      return true;
    }
  }
}
  // ------------------------------------------ Vertical Speed Stable ------------------------------------------ //
  bool baroStab() {
    if (fabs(vSpeed) > 0.2  || bStab == false) {
      tStab = millis();                                              // Reset timer
    }
    if (millis() - tStab > BARO_STABLE) {                                   // If timer hasnt reset within 1 second altitude is stable
      bStab = false;                                               // Reset started flag
      return true;
    }
    else {
      bStab = true;
      return false;
    }
  }
  // ------------------------------------------ Update  Vertical Speed ------------------------------------------- //
  void updateVerticalSpeed() {
    if (millis() - tSpeed >= 100) {
      float da = baroAlt - prevBaroAlt;
      vSpeed = (da / 0.1);  // mm/s
      vSpeed = vSpeedAvg.reading(vSpeed);                  // Commit new reading to moving average array and read the average
      prevBaroAlt = baroAlt;
      tSpeed = millis();
    }
  }
