/*
   You can connect an external voltage directly to the BATT_VOLT_PIN as long as the the pin is rated for that voltage
   Alteratively, you can use a voltage divider so you can connect a higher voltage and use R1 and R2

   Voltage divider schema:

                           |----------------|
                           |               R1
                     (+) --|                |
                   Battery                  |------ To BATTERY_VOLTAGE_PIN
                     (-) --|                |
                           |               R2
                           |----------------|------ To GND

    DIVIDER_RATIO can be calculated by (R1+R2) / R2
    Optionally add a 100nF capacitor between A1 and GND if the measured voltage seems unstable
*/

//movingAvgFloat voltage (10);

unsigned long tBatt = 0;      // last battery update time
float readVolt = 0.00;
float avgVolt = 0.00;
float battVolt = 0.00;
float dividerRatio = 0.00;

enum batteryState {
  BATTERY_OK,
  BATTERY_LOW,
  BATTERY_CRITICAL
} batteryState;

void setupBatt() {
  pinMode(BATT_VOLT_PIN, INPUT);
  dividerRatio = (RESISTOR_1 + RESISTOR_2) / RESISTOR_2;
#ifdef DEVMODE
  Serial.println("Battery setup complete");
#endif
}

// ------------------------------------------ Battery Functions ------------------------------------------- //

void updateBattery(int t) {

  if ((millis() - tBatt) >= t) {
    tBatt = millis();

    readVolt = analogRead(BATT_VOLT_PIN);
 //   avgVolt = voltage.reading(readVolt);
    battVolt = (readVolt * dividerRatio) + BATT_VOLT_OFFSET;
 //   battVolt = (avgVolt * dividerRatio) + BATT_VOLT_OFFSET;

    switch (batteryState) {
      case BATTERY_OK:
        {
          if (battVolt < BATT_VOLT_LOW) {
            batteryLowBuzzer();
            batteryState = BATTERY_LOW;
#ifdef DEVMODE
            Serial.println("Battery low");
#endif
          }
        }
        break;

      case BATTERY_LOW:
        {
          if (battVolt < BATT_VOLT_CRITICAL) {
            batteryCriticalBuzzer();
            batteryState = BATTERY_CRITICAL;
#ifdef DEVMODE
            Serial.println("Battery critical");
#endif
          }
          else {
            batteryState = BATTERY_OK;
            buzzerOff();
#ifdef DEVMODE
            Serial.println("Battery OK");
#endif
          }
        }
        break;

      case BATTERY_CRITICAL:
        {
          if (battVolt > BATT_VOLT_LOW) {
            batteryState = BATTERY_OK;
            buzzerOff();
#ifdef DEVMODE
            Serial.println("Battery OK");
#endif
          }
        }
        break;
    }
  }
}
