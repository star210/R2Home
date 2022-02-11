//movingAvg mult(10);       //Gain multipler for cog weight, if cog is higher than threshold
//movingAvg rateErr(1);
//movingAvgFloat rateTurn(2);     //Turn speed of wing, derivative of cog

float prevGpsAlt = 0.0;
int cogCount = 0;
bool newCog = false;
float prevCog = 0.0;
float prevCogB = 0.0;
float rateCog = 0.0;
float angleHome = 0.0;        // Angle from
float errorHome = 0.0;
float lastErrorHome = 0.0;
float rateError = 0.0;
float commandHome = 0;     // Servo command in degrees
bool spiral = false;

void navigation() {
  if (gps.course.isUpdated()) {

TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), settings.homeLatitude, settings.homeLongitude) / 1000.0;

    newCog = true;
    if (cogValid(COG_COUNT) == true) {
      angleHome = TinyGPSPlus::courseTo(gps.location.lat(), gps.location.lng(), settings.homeLatitude, settings.homeLongitude);
      errorHome = getAngle(gps.course.deg(), angleHome);
    }
    else {
      lastErrorHome = errorHome;
    }
    prevCog = gps.course.deg();
  }
  newCog = false;


  if (GPS_OK() && cogValid(COG_COUNT) == true) {

    if (newCog == true) {
      rateError = getAngle(lastErrorHome + 180, errorHome + 180) * GPS_FREQ;
   //   rateError = rateErr.reading(rateError);

      rateCog  = getAngle(prevCogB, gps.course.deg()) * GPS_FREQ;
    //  rateCog  = rateTurn.reading(rateCog);

      float PIDsum = (Kp * errorHome) + (Kd * rateError);

      //cmd_mult = (0.09999996 + (1 - 0.09999996)/(1 + pow(((abs(ratecog))/49.54231),24.26521))); cmd_mult = mult.reading(cmd_mult*1000); cmd_mult = (cmd_mult/1000);

      int commandMulti = 1;
      commandHome  = PIDsum * commandMulti;

      if (vSpeed < -5) {
        spiral = true;
      }
      if (vSpeed > -3) {
        spiral = false;
      }
      if (spiral == true) {
        commandHome = 0;
      }

      newCog = false;
      lastErrorHome = errorHome;
      prevCogB = gps.course.deg();
    }
  }
  else {
    commandHome = 0;
  }
}

// ------------------------------------------------ Calc Angle --------------------------------------------------- //
float getAngle(float a, float b) {
  float angle = 0;
  if (abs(a - b) < 180) {
    angle = (b - a);
  }
  else {
    if ((a - b) < 0) {
      angle = (-360) + (b - a);
    }
    else {
      angle = (360) + (b - a);
    }
  }
  return angle;
}

// ----------------------------------------------------- COG Stable ------------------------------------------------------ //
bool cogValid(int a) {
  if (abs(gps.course.deg() - prevCog) < 0.1) {
    cogCount ++;
  }
  else {
    cogCount = 0;
  }
  if (cogCount >= a) {
    return false;
  }
  else {
    return true;
  }
}
