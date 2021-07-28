
#include "StatusLed.h"

void StatusLed::init() {
    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(10);
    leds[0] = CRGB::Red;
    FastLED.show();
}

void StatusLed::tick() {
}