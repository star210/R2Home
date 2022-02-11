#include "config.h"
#include <Watchdog.h>
//#include <movingAvgFloat.h>
#include <movingAvg.h>
#include <EasyBuzzer.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

TinyGPSPlus gps;
Watchdog watchdog;

struct _Data {
  float homeLatitude;       // Save home Lat
  float homeLongitude;      // Save home Long
  float homeAltitude;       // Save home gps altitude
  uint16_t homePressure;       // Baro calibration
};
struct _Data settings;

float vSpeed = 0;            // make a function to remove

uint16_t tLoop = 0;
uint16_t loopTime = 0;

enum State {
  INITIALISE,
  READY,
  ASCENT,
  DESCENT,
  GLIDING_AUTO,
  GLIDING_MANUAL,
  LANDED
} flightMode;

void setup() {

#ifdef DEVMODE
  Serial.begin(115200);
#endif
  watchdog.enable(Watchdog::TIMEOUT_1S);

  setupBaro();
  setupBatt();
  setupGPS();
  setupRadio();
  setupCam();
  setupBuzzer();
  setupTelemetry();
}

void loop() {

  tLoop = micros();       // Start loop timer
  updateGPS();
  computeBaroAlt(50);         // Update barometric pressure every 50ms
  updateBattery(500);     // Update battery every 500 ms

  flightCompute();         // Loop to run flight computer

  //updateled();
  EasyBuzzer.update();
  loopTime = micros() - tLoop;
}

// ------------------------------------------ Flight Computer ------------------------------------------- //

void transition(State nextState) {
  flightMode = nextState;
  // Mode has changed so transmit telemetry and write to SD
  EasyBuzzer.beep(3000, 100, 50, 5, 500, 1);
  sendTelemetry();
  writeToSD();
#ifdef DEVMODE
  Serial.print("Transitioning to: ");
  Serial.println(nextState);
#endif
}

void flightCompute() {

  switch (flightMode) {

    case INITIALISE:

      if (DROP == true) {
        if (setGpsHome() == true && setBaroSeaLevel() == true) {

          writeSettings();          // Save settings to EEPROM

          // Create new SD card file if the launch and ascent are set to be recorded only when gps has a lock to create a file name
          if (RECORD_LAUNCH == true) {
            createNewFile();        // create new SD card file
            setWriteToSD(200);      // set to record data to SD card every 200 ms
          }

          setCamera(1);             // Switch on camera
          setTelemetry(1000);       // Set telemetry transmit timer
          resetBaroAvg();
          transition(READY);
        }
        else {
          satelliteCount();            // loop satellite count untill min amount of satellites have been found
        }
      }
      transition(GLIDING_MANUAL);      // If drop is false goto manual mode
      break;

    case READY:
      // Flight ready and waiting for vertical speed change
      if (GPS_OK() == false)  {
        transition(INITIALISE);
      }
      if (fabs(vSpeed) > 2) {
        if (vSpeed > 0) {
          transition(ASCENT);
        }
        else {
          transition(DESCENT);
        }
      }
      break;

    case ASCENT:
      if (vSpeed < 0.5) {
        transition(READY);
      }
      break;

    case DESCENT:
      if (vSpeed > -0.5) {
        transition(READY);
      }
      // if (baroAlt < DEPLOY_AGL) {           make into function to remove from main
      deployWing();
      transition(GLIDING_AUTO);
      //   }
      break;

    case GLIDING_AUTO:
      if (failSafe() == false) {
        transition(GLIDING_MANUAL);
      }
      updateWing(50);
      navigation();
      break;

    case LANDED:
      //  if (baroAlt < 5) {                                // Make into a function to remove from main
      if (baroStab() == true || gpsStab == true) {
        transition(INITIALISE);
      }
      //    }
      break;
  }
}

// --------------------------------------- Fusion of GPS and Baro Altitudes --------------------------------------- //

//baro_weight = (1 + (abs(vspeed) / 10));
//
//float hdo = gps.hdop.value();
//float gpsw = ((50 / hdo) + (gps.altitude.meters() / 10000));
//gps_weight = (gpsw * ((abs(alt_baro - gps.altitude.meters()) / 10)));
//
//merged_alt = ((alt_baro * baro_weight) + (gps.altitude.meters() * gps_weight)) / (baro_weight + gps_weight);
