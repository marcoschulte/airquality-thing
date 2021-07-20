
#ifndef AIRQUALITY_ESP32_SENSORS_H
#define AIRQUALITY_ESP32_SENSORS_H

#include <Arduino.h>
#include "CO2_S8.h"
#include "PM_PMS5003.h"

class Sensors {
public:
    struct SensorValues {
        int co2;
        uint16_t pm1_0, pm2_5, pm10_0;
    };

    void init();

    void tick();

    Sensors::SensorValues *values();

private:
    void resetValues();

    void updateCO2();

    SensorValues sensorValues;

    CO2_S8 co2S8 = CO2_S8(D4, D3);

    PM_PMS5003 pm = PM_PMS5003(D5, D6);
    bool pmEnabled = false;
    unsigned long lastPmRead = 1LL << 31;;
    unsigned long pmWarmupStartedAt = 0;

    void updatePM();

    void doReadPM();
};


#endif //AIRQUALITY_ESP32_SENSORS_H
