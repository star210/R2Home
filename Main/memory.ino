#include <SD.h>
#include <EEPROM.h>
#include <SPI.h>

File dataFile;

unsigned long tRecord = 0;      // SD write last time recorded
char fileName[13];

// ------------------------------------------------------ Memory Functions ------------------------------------------------------ //

  bool initSD() {
    // see if the card is present and can be initialized:
    if (!SD.begin(CS_PIN)) {
#if defined(DEVMODE)
      Serial.println("SD card failed, or not present");
#endif
      return false;
    }
#if defined(DEVMODE)
    Serial.println("SD card initialized");
#endif
    return true;
  }

  bool createNewFile() {
    if (initSD() == true) {
      // Create filename from date and time from GPS
      sprintf(fileName, "%02d%02d%02d%02d%02d%02d.csv", gps.date.year(), gps.date.month(), gps.date.day(), gps.time.hour(), gps.time.minute(), gps.time.second() );

      // Create a new file and write the header text
      dataFile = SD.open(fileName, FILE_WRITE);
      if (dataFile) {
        dataFile.println("time (ms), Packet_Count (text), Mode (text), GPS_Ok (text), COG_Ok (text), FailSafe (text), GPS_Stab (text), Baro_Stab (text), Deployed (text), Wing_Opened (text), Initialised (tex), Vbatt (V), Loopmin (µS), Loopmax (µS), Loopmean (µS), GPS-date, GPS-time, lat (deg), lon (deg), alt (m), CoG (deg), Speed (m/s), Sat_in_use (text), HDOP (text), Position_Age (text), Fix_type (text), Baro_Alt (m), Vertical_Speed (m/s), Altitude (m), Baro_Weight (text), GPS_Weight (text), SetPoint_Home (deg), Err_Home (deg), Rate_Error (deg), Next_Cog (deg), Cmd_mult (text), LatB (deg), LonB (deg), WaypointNumber (text), Distance (m), Ch 0 (µs), Ch 1 (µs), Ch 2 (µs), Ch 3 (µs), Ch 4 (µs), Ch 5 (µs), Ch 6 (µs), PWM_L (µs), PWM_R (µs), PWM_D (µs)");
        dataFile.close();
#if defined(DEVMODE)
        Serial.println("SD card file column headers written");
#endif
      }
    }
  }

  void writeToSD() {
    if (initSD() == true && SD_WRITE_ENABLED == true) {
      dataFile = SD.open(fileName, FILE_WRITE);
     // dataFile.println(mainSD);
      dataFile.close();
    }
  }

  void setWriteToSD(int t) {
    if ((millis() - tRecord) >= t) {
      tRecord = millis();
      writeToSD();
    }
  }

  void readSettings() {
    EEPROM.get(SETTINGS_EEPROM_ADDR, settings);
#if defined(DEVMODE)
    Serial.println("EEPROM Settings read");
#endif
  }

  void writeSettings() {
    EEPROM.put(SETTINGS_EEPROM_ADDR, settings);
#if defined(DEVMODE)
    Serial.println("EEPROM Settings written");
#endif
  }

  void wipeEeprom() {
    for ( unsigned int i = 0 ; i < EEPROM.length() ; i++ )
      EEPROM.write(i, 0);
#if defined(DEVMODE)
    Serial.println("EEPROM Settings erased");
#endif
  }
