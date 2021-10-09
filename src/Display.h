
#ifndef AIRQUALITY_ESP32_DISPLAY_H
#define AIRQUALITY_ESP32_DISPLAY_H

#include <SSD1306Wire.h>
#include "Sensors.h"
#include "NightMode.h"

class Display {
public:
    Display(Sensors *sensors, NightMode *nightMode) : sensors(sensors), nightMode(nightMode) {};

    void init();

    void tick();

private:
    SSD1306Wire display = SSD1306Wire(0x3c, SDA, SCL);
    Sensors *sensors;
    NightMode *nightMode;
    int frame = 0;
    unsigned long lastFrame = 1LL << 31;
    bool isDisplayOn = true;

    void displayFrame1();

    void displayFrame2();
};


#endif //AIRQUALITY_ESP32_DISPLAY_H
