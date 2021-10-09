
#include "StatusLed.h"

#define BEST_HUE (uint16_t) 21500
#define WORST_HUE (uint16_t) 60000

#include "user_interface.h"

static os_timer_t myTimer;

static StatusLed *staticStatusLed;

void timerCallback(void *pArg) {
    staticStatusLed->internalTick();
}

void StatusLed::init() {
    staticStatusLed = this;

    pixels.begin();
    pixels.setPixelColor(0, pixels.ColorHSV(100, 100, 100));
    pixels.show();

    os_timer_setfn(&myTimer, timerCallback, NULL);
    os_timer_arm(&myTimer, 30, true);
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
        hueSetpoint = aqiToHue(num);
    }
}

int32_t toSignedHue(uint16_t hue) {
    return (int32_t) (uint16_t) (hue - WORST_HUE);
}

uint16_t toUnsignedHue(int32_t hue) {
    return (uint16_t) (hue + WORST_HUE);
}

void StatusLed::internalTick() {
    hueSetpoint = aqiToHue(sensors->values()->aqiMax);
    satSetpoint = 255;
    valSetpoint = nightMode->isNight() ? 3 : 50;

    int32_t sHueSet = toSignedHue(hueSetpoint);
    int32_t sHueCur = toSignedHue(hueCur);
    int32_t dif = sHueSet - sHueCur;

    int32_t nextHue = sHueCur + dif * 0.03;

    hueCur = toUnsignedHue(nextHue);
    satCur += (satSetpoint - satCur) * 0.03;
    valCur += (valSetpoint - valCur) * 0.03;

    pixels.setPixelColor(0, pixels.ColorHSV(hueCur, (uint16_t) satCur, (uint16_t) valCur));
    pixels.show();
}
