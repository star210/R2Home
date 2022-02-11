#include <PPMReader.h>
#include <PWMServo.h>

PPMReader ppm(PPM_PIN, PPM_CHANNELS);

PWMServo wing;
PWMServo deploy;
PWMServo esc;
PWMServo left;
PWMServo right;

// Radio
int servoAuto = 1500;
int servoWing = 1500;
int servoDeploy = 2000;
int servoEsc = 1000;
unsigned long tPos = 0;
int pos = 0;
bool dir = false;
unsigned long tWing = 0;
//bool failSafe = true;

void setupRadio() {

#ifdef DROP
  deploy.attach(DEPLOY_PIN, 800, 2200);
  deploy.write(DEPLOY_SAFE);

#else
  esc.attach(ESC_PIN, 800, 2200);
  esc.write(0);
#endif

#ifdef HANDS_UP
  left.attach(LEFT_PIN, 800, 2200);
  left.write(90);
  right.attach(RIGHT_PIN, 800, 2200);
  right.write(90);
#else
  wing.attach(WING_PIN, 800, 2200);
  wing.write(90); // Wing center
#endif

#ifdef DEVMODE
  Serial.println("Radio setup complete");
#endif
}

// ------------------------------------------------------ Radio Functions ------------------------------------------------------ //

bool failSafe() {
  return false;
}

void updatePPM() {

  for (int channel = 1; channel <= PPM_CHANNELS; ++channel) {
    unsigned long value = ppm.latestValidChannelValue(channel, FAILSAFE_VALUE);
    // Serial.print(String(value) + " ");
  }
}

void deployWing() {
  deploy.write(DEPLOY_WING);
}

void updateWing(int t) {
  if (millis() - tWing >= t) {
    tWing = millis();
    servoAuto = map(commandHome, -180, +180, 0, 180);

    switch (SERVO_MODE) {
      case 0: // Steer with 1 servo in opposite mode
        wing.write(servoAuto);
        break;
      case 1: // Steer with 2 servos in opposite mode
        right.write(servoAuto);
        left.write(180 - servoAuto);
        break;
      case 2: // Steer with 2 servos in hands up mode
        if (servoAuto <= 90) {    // Turn left
          left.write(servoAuto);
          right.write(90);
        }
        else {                    // Turn right
          left.write(90);
          right.write(180 - servoAuto);
        }
        break;
    }
  }
}
  // ------------------------------------------------------ Servo Test ------------------------------------------------------ //

  int servoTest(int _min, int _max, int _increment, int _delay) {
    if (millis() - tPos >= _delay) {
      if (pos <= _min) {
        dir = true;
      }
      if (pos >= _max) {
        dir = false;
      }
      if (dir == true) {
        pos += _increment;
      }
      else {
        pos -= _increment;
      }
      tPos = millis();
      return pos;
    }
  }
