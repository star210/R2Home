//#define DEVMODE                            // Uncomment to use serial print

//**********************************************************************************************************************
// Flight settings
#define DROP                 true          // R2Home's version, drop true or motorised false
//#define HANDS_UP                         // Control linear or with "hands up"
#define DEPLOY_AGL           100           // m above ground level
#define SERVO_MODE           0             // 0 1 2 
#define COG_COUNT            2             // Course over ground counter "nav"

#define Kp  1
#define Kd  0

//**********************************************************************************************************************
// Serial Ports
//#define serialTelem          Serial5       // Serial port for telemetry
#define TELEM_BAUD           57600         // Baud rate for telemetery

//**********************************************************************************************************************
// RC Settings
#define PPM_PIN              7             // Pin 3 - ISR1 on arduino Atmega328
#define PPM_CHANNELS         6             // Total PPM channels
#define FAILSAFE_VALUE       1500          // Channel value if failsafe condition

#define DEPLOY_PIN           9             // Chute deploy servo pin 
#define DEPLOY_WING          0             // Deploy servo minimum setting in degrees
#define DEPLOY_SAFE          180           // Deploy servo maximum setting in degrees

#define ESC_PIN              9             // ESC pin
#define ESC_MIN              0             // ESC minimum setting
#define ESC_MAX              180           // ESC maximum setting

#define WING_PIN             10            // Single servo is used as a push and pull
#define WING_CENTER_POS      1500          // 
#define LEFT_PIN             5             // Wing left brake servo
#define RIGHT_PIN            2             // Wing right brake servo
#define STEER_MODE           0             // 0 Steer opposite 1 servo mode, 1 Steer opposite with 2 servos mode, 2 Steer in hands up mode

#define i_want_to_fly        false         // Simulated servo movement to test the servo movement :))

//**********************************************************************************************************************
// GPS Settings
#define GPS_TX_PIN           3             // Software serial for GPS TX pin
#define GPS_RX_PIN           4             // Software serial for GPS RX pin
//#define GPS_COG_FILTER       0             // COG filter (0 = Off, 1 = On)
//#define GPS_NMEA             1             // 
#define MODE_2               2             // Mode (1, 2 or 4)
#define GPS_BAUD_INIT        9600          // Gps initial baud rate
#define BAUD_57600                         // Gps operating baud rate
#define GPS_MIN_SATS         6             // Minimum gps satellites to get a accurate position
#define GPS_MIN_AGE          200           // Minimum location age in ms
#define GPS_FREQ             5             // Hz (1, 5 or 10)
#define GPS_MIN_HDOP         300           // Gps HDOP value 0 - 1000

//**********************************************************************************************************************
// Memory
#define SD_WRITE_ENABLED     true          // Should it write on the SD card or not? 
#define RECORD_LAUNCH        false         // only record autopilot 
#define CS_PIN               4             // Chip select pin 
#define SETTINGS_EEPROM_ADDR 10            // Start address for eeprom settings
//#define WIPE_EEPROM                      // Uncomment to erase the eeprom

//**********************************************************************************************************************
// Barometer
#define BARO_ADDRESS         0x76          // I2C address of BMP180
#define BARO_MSLP            1013          // Mean Sea Level Pressure on the day in mBar

//**********************************************************************************************************************
// Camera
#define CAMERA_POWER_PIN     A2            // Pin for to switch on camera          

//**********************************************************************************************************************
// Buzzer
#define BUZZER_PIN           6

//**********************************************************************************************************************
// Led Indicator
#define DATA_PIN             3
#define NUM_PIXELS           1

//**********************************************************************************************************************
// Battery
#define BATT_VOLT_PIN        A0            // Analog read pin 
#define BATT_VOLT_LOW        3.5            // Volts
#define BATT_VOLT_CRITICAL   3.4            // Volts
#define RESISTOR_1           680            // Ohms use 0 if not used
#define RESISTOR_2           220            // Ohms use 0 if not used
#define BATT_VOLT_OFFSET     0.00           // Offset for error correction in battery read volts
 
