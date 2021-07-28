
#ifndef AIRQUALITY_ESP32_STATUSLED_H
#define AIRQUALITY_ESP32_STATUSLED_H

#include <FastLED.h>
#include "Sensors.h"

#define LED_PIN     D7
#define NUM_LEDS    1

class StatusLed {
public:
    StatusLed(Sensors *sensors) : sensors(sensors) {};

    void init();

    void tick();

private:
    Sensors *sensors;
    CRGB leds[NUM_LEDS];
};


#endif //AIRQUALITY_ESP32_STATUSLED_H
