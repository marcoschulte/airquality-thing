
#include "StatusLed.h"

Adafruit_NeoPixel *pixels;

#define BEST_HUE (uint16_t) 21500
#define WORST_HUE (uint16_t) 60000

void StatusLed::init() {
    pixels.begin();
    pixels.setPixelColor(0, pixels.ColorHSV(100, 100, 100));
    pixels.show();
}

float StatusLed::easeOutCirc(float x) {
    return sqrt(1 - pow(x - 1, 2));
}

uint16_t StatusLed::aqiToHue(int aqi) {
    float normalized = max(min((double) aqi, 300.) - 50, 0.) / 250;
    normalized = easeOutCirc(normalized);

    uint16_t range = BEST_HUE - WORST_HUE;
    uint16_t hue = BEST_HUE - range * normalized;

    return hue;
}

void StatusLed::readAQIFromSerial() {
    if (Serial.available() > 0) {
        delay(20);

        int num = 0;
        while (Serial.available() > 0) {
            char c = char(Serial.read());
            if (c >= '0' && c <= '9') {
                num *= 10;
                num += c - '0';
            }
        }

        uint16_t hue = aqiToHue(num);

        Serial.println("We got: " + String(num) + ", hue " + String(hue));
        pixels.setPixelColor(0, pixels.ColorHSV(hue, 255, 255));
        pixels.show();
    }
}

void StatusLed::tick() {
    readAQIFromSerial();
}