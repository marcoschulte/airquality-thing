
#ifndef AIRQUALITY_ESP32_STATUSLED_H
#define AIRQUALITY_ESP32_STATUSLED_H

#include <Adafruit_NeoPixel.h>
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
    Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
};


#endif //AIRQUALITY_ESP32_STATUSLED_H
