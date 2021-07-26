
#ifndef AIRQUALITY_ESP32_DISPLAY_H
#define AIRQUALITY_ESP32_DISPLAY_H

#include <SSD1306Wire.h>
#include "Sensors.h"

class Display {
public:
    Display(Sensors *sensors) : sensors(sensors) {};

    void init();

    void tick();

private:
    SSD1306Wire display = SSD1306Wire(0x3c, SDA, SCL);
    Sensors *sensors;
    int frame = 0;
    unsigned long lastFrame = 1LL << 31;

    void displayFrame1();

    void displayFrame2();
};


#endif //AIRQUALITY_ESP32_DISPLAY_H
