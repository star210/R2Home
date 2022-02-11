#define DEVMODE

// Barometer
#define BARO_ADDRESS         0x76          // I2C address of BMP180
#define BARO_UPDATE 10
#define BARO_STABLE          1000

// Memory
#define SD_WRITE_ENABLED     true          // Should it write on the SD card or not? 
#define RECORD_LAUNCH        false         // only record autopilot 
#define CS_PIN               8             // Chip select pin 
#define SETTINGS_EEPROM_ADDR 10            // Start address for eeprom settings
//#define WIPE_EEPROM                      // Uncomment to erase the eeprom

// GPS Settings
#define GPS_STABLE           1000
#define GPS_TX_PIN           3             // Software serial for GPS TX pin
#define GPS_RX_PIN           4             // Software serial for GPS RX pin

#define BAUD_TEST
//#define COG_FILTER                   // COG filter (0 = Off, 1 = On)
//#define NMEA

// Gps mode
//#define MODE_1
//#define MODE_2
//#define MODE_4

// Gps update rate
//#define FREQ_1_HZ
//#define FREQ_5_HZ
//#define FREQ_10_HZ

// Gps operating baud rate
//#define BAUD_9600
//#define BAUD_57600
//#define BAUD_115200
                         
#define GPS_BAUD_INIT        9600          // Gps initial baud rate
#define GPS_MIN_SATS         6             // Minimum gps satellites to get a accurate position
#define GPS_MIN_AGE          100           // Minimum location age in ms
#define GPS_MIN_HDOP         80           // Gps HDOP value 0 - 1000
