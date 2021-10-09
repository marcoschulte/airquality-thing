
#include "NightMode.h"

#define EZTIME_EZT_NAMESPACE

#include <Arduino.h>
#include <ezTime.h>

Timezone timezoneBerlin;

void NightMode::init() {
    ezt::waitForSync();
    timezoneBerlin.setLocation("Europe/Berlin");
    ezt::setInterval(UPDATE_INTERVAL_SECONDS);
    initialized = true;
}

bool NightMode::isNight() {
    return night;
}

void NightMode::tick() {
    night = determineNight();
}

bool NightMode::determineNight() {
    if (initialized) {
        ezt::waitForSync(10);

        if (ezt::timeStatus() == timeSet) {
            tmElements_t tm;
            ezt::breakTime(timezoneBerlin.now(), tm);

            return tm.Hour >= 23 || tm.Hour < 8;
        }
    }

    return false;
}
