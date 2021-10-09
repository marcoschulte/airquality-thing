
#ifndef AIRQUALITY_ESP32_STATUSLED_H
#define AIRQUALITY_ESP32_STATUSLED_H

#include <Adafruit_NeoPixel.h>
#include "Sensors.h"
#include "NightMode.h"

#define LED_PIN     D7
#define NUM_LEDS    1

class StatusLed {
public:
    StatusLed(Sensors *sensors, NightMode *nightMode) : sensors(sensors), nightMode(nightMode) {};

    void init();

    void internalTick();

private:
    Sensors *sensors;
    NightMode *nightMode;
    Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

    uint16_t hueSetpoint, hueCur = 100;
    float satSetpoint, satCur = 100;
    float valSetpoint, valCur = 100;

    float easeOutCirc(float x);

    void readAQIFromSerial();

    uint16_t aqiToHue(int aqi);
};


#endif //AIRQUALITY_ESP32_STATUSLED_H
