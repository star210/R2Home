// EasyBuzzer.beep(frequency, onDuration, offDuration, beeps, pauseDuration, cycles);

uint16_t tSat = 0;     // Sat buzzer timer

void setupBuzzer() {
  pinMode(BUZZER_PIN, OUTPUT);
  EasyBuzzer.setPin(BUZZER_PIN);
  // Play setup finished sound
  EasyBuzzer.singleBeep(523, 200);
  EasyBuzzer.singleBeep(582, 200);
  EasyBuzzer.singleBeep(762, 300);
#ifdef DEVMODE
  Serial.println("Buzzer setup complete");
#endif
}

// ------------------------------------------ Buzzer Functions ------------------------------------------- //

void batteryLowBuzzer() {
  EasyBuzzer.setOnDuration(300);  // Set On duration.
  EasyBuzzer.setOffDuration(1700);  // Set Off duration.
  EasyBuzzer.beep(2000);
}

void batteryCriticalBuzzer() {
  EasyBuzzer.setOnDuration(500);  // Set On duration.
  EasyBuzzer.setOffDuration(500);  // Set Off duration.
  EasyBuzzer.beep(3000);
}

void buzzerOff() {
  EasyBuzzer.stopBeep();
}

void satelliteCount() {
  if (millis() - tSat > 5000) {
    tSat = millis();
    EasyBuzzer.beep(2000, 50, 25, gps.satellites.value(), 100, 1);
  }
}
