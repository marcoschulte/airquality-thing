
#ifndef AIRQUALITY_ESP32_SENSORS_H
#define AIRQUALITY_ESP32_SENSORS_H

#include <Arduino.h>
#include "CO2_S8.h"
#include "PM_PMS5003.h"
#include "BME680.h"

class Sensors {
public:
    struct SensorValues {
        int co2;
        uint16_t pm1_0, pm2_5, pm10_0;
        float temperature, pressure, pressureSeaLevel, humidity, staticIaq, breathVocEquivalent;
    };

    void init();

    bool tick();

    Sensors::SensorValues *values();

private:
    SensorValues sensorValues;

    void resetValues();

    bool updateCO2();

    unsigned long lastCO2Read = 1LL << 31;
    CO2_S8 co2S8 = CO2_S8(D4, D3);


    bool updatePM();

    bool doReadPM();

    PM_PMS5003 pm = PM_PMS5003(D5, D6);
    bool pmEnabled = false;
    unsigned long lastPmRead = 1LL << 31;
    unsigned long pmWarmupStartedAt = 0;

    BME680 bme680;

    bool updateBME();

    unsigned long lastBmeRead = 1LL << 31;

    float calcPressureSeaLevel(float pressure, float temperature);
};


#endif //AIRQUALITY_ESP32_SENSORS_H
