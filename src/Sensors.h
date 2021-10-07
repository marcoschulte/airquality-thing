
#ifndef AIRQUALITY_ESP32_SENSORS_H
#define AIRQUALITY_ESP32_SENSORS_H

#include <Arduino.h>
#include "CO2_S8.h"
#include "PM_PMS5003.h"
#include "BME680.h"
#include "AQICalculator.h"
#include "SHT.h"

class Sensors {
public:
    typedef enum {
        OK = 0,
        NOK = -1
    } sensor_status_t;

    struct SensorValues {
        int co2;
        uint16_t pm1_0, pm2_5, pm10_0;
        float temperature_bme680, temperature_sht3x, pressure, pressureSeaLevel, humidity_bme680, humidity_sht3x, voc, aqiVoc, aqiPM2_5, aqiPM10_0, aqiCO2, aqiMax;
        sensor_status_t co2Status, bmeStatus, pmsStatus, shtStatus;
    };

    void init();

    bool tick();

    Sensors::SensorValues *values();

private:
    SensorValues sensorValues;
    AQICalculator aqiCalculator;

    void resetValues();

    // PMS stuff
    bool updateCO2();

    unsigned long lastCO2Read = 1LL << 31;
    CO2_S8 co2S8 = CO2_S8(D4, D3);

    // PM stuff
    bool updatePM();

    bool doReadPM();

    PM_PMS5003 pm = PM_PMS5003(D5, D6);
    bool pmEnabled = false;
    unsigned long lastPmRead = 1LL << 31;
    unsigned long pmWarmupStartedAt = 0;

    // BME stuff
    BME680 bme680;

    bool updateBME();

    unsigned long lastBmeRead = 1LL << 31;

    // SHT stuff
    SHT sht;

    bool updateSHT();

    unsigned long lastSHTRead = 1LL << 31;

    // other
    float calcPressureSeaLevel(float pressure, float temperature);

    void updateAQIMax();
};


#endif //AIRQUALITY_ESP32_SENSORS_H
