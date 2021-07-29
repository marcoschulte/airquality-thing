
#include "StatusLed.h"

Adafruit_NeoPixel *pixels;

void StatusLed::init() {
    pixels.begin();
    pixels.setPixelColor(0, pixels.ColorHSV(100, 100, 100));
    pixels.show();
}

void StatusLed::tick() {
    pixels.setPixelColor(0, pixels.ColorHSV(100, 100, 100));
    pixels.show();
}