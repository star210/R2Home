void setupCam () {
  pinMode(CAMERA_POWER_PIN, OUTPUT);
  digitalWrite(CAMERA_POWER_PIN, LOW);

#ifdef DEVMODE
  Serial.println("Camera setup complete");
#endif
}


// ------------------------------------------ Camera Functions ------------------------------------------- //

void setCamera(int cam) {
  switch (cam) {
    case 1 :
      digitalWrite(CAMERA_POWER_PIN, HIGH);
#ifdef DEVMODE
      Serial.println("Camera ON");
#endif
      break;
    case 0 :
      digitalWrite(CAMERA_POWER_PIN, LOW);
#ifdef DEVMODE
      Serial.println("Camera OFF");
#endif
      break;
  }
}
