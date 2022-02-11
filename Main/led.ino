//#include <Adafruit_NeoPixel.h>
//
//Adafruit_NeoPixel strip(NUM_PIXELS, DATA_PIN, NEO_GRBW + NEO_KHZ800);
//unsigned long tLed = 0;
//
//void setLedColor(int red, int green, int blue, int timeOn, int e) {
//
//  if ((millis() - tLed) >= e) {
//    lastled = millis();
//    colorWipe(strip.Color(red, green, blue), 0);
//    duration = timeOn;
//    timeled = millis();
//  }
//}
//
//void updateled() {
//  if ((millis() - timeled) >= duration) {
//    colorWipe(strip.Color(0, 0, 0), 0);
//  }
//}
//
//void colorWipe(uint32_t color, int wait) {
//  for (int i = 0; i < strip.numPixels(); i++) {
//    strip.setPixelColor(i, color);
//    strip.show();
//  }
//}
