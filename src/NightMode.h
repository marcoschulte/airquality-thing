
#ifndef AIRQUALITY_ESP32_NIGHTMODE_H
#define AIRQUALITY_ESP32_NIGHTMODE_H

#define UPDATE_INTERVAL_SECONDS 24 * 60 * 60

class NightMode {

public:
    void init();

    bool isNight();

    void tick();

private:

    bool initialized = false;
    bool night = false;

    bool determineNight();

};


#endif //AIRQUALITY_ESP32_NIGHTMODE_H
