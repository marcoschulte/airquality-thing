#include "Sensors.h"

#define HEIGHT_ABOVE_SEA_LEVEL 525

#define CO2_EVERY_MS 3 * 1000

#define PM_EVERY_MS 300 * 1000
#define PM_WARMUP_MS 30 * 1000

#define BME_EVERY_MS 3 * 1000

#define SHT_EVERY_MS 3 * 1000

void Sensors::init() {
    resetValues();
    co2S8.init();
    pm.init();
    pm.setModePassive();
    bme680.init();
}

bool Sensors::tick() {
    bool updated = false;

    // put bme first, as timing for bme is critical
    updated = updateBME() || updated;
    updated = updateCO2() || updated;
    updated = updatePM() || updated;
    updated = updateSHT() || updated;

    updateAQIMax();

    return updated;
}

bool Sensors::updateCO2() {
    if (millis() - lastCO2Read > CO2_EVERY_MS) {
        int co2 = co2S8.read();
        if (co2 == -1) {
            Serial.println("S8 read failure");
            sensorValues.co2 = NULL;
            sensorValues.aqiCO2 = NULL;
            sensorValues.co2Status = NOK;
        } else {
            sensorValues.co2 = co2;
            sensorValues.aqiCO2 = aqiCalculator.calculateAQI(sensorValues.co2, aqiCalculator.thresholdsCO2);
            sensorValues.co2Status = OK;
            lastCO2Read = millis();
            return true;
        }
    }

    return false;
}

bool Sensors::updatePM() {
    if (pmEnabled) {
        if (millis() - pmWarmupStartedAt > PM_WARMUP_MS) {
            Serial.println("PM read");

            bool success = doReadPM();

            Serial.println("PM sleep");
            pm.sleep();
            pmEnabled = false;
            lastPmRead = millis();
            return success;
        }
    } else {
        if (millis() - lastPmRead > PM_EVERY_MS) {
            Serial.println("PM warm-up");
            pm.wake();
            pmEnabled = true;
            pmWarmupStartedAt = millis();
        }
    }

    return false;
}

bool Sensors::doReadPM() {
    PM_PMS5003::data data;
    for (int i = 0; i < 3; i++) {
        if (pm.read(&data)) {
            if (data.particles_0_3um == 0) {
                continue;
            }

            sensorValues.pm1_0 = data.pm1_0_std;
            sensorValues.pm2_5 = data.pm2_5_std;
            sensorValues.pm10_0 = data.pm10_0_std;
            sensorValues.aqiPM2_5 = aqiCalculator.calculateAQI(sensorValues.pm2_5, aqiCalculator.thresholdsPM2_5);
            sensorValues.aqiPM10_0 = aqiCalculator.calculateAQI(sensorValues.pm10_0, aqiCalculator.thresholdsPM10_0);
            sensorValues.pmsStatus = OK;

            return true;
        } else {
            Serial.println("PM read failure");
        }
    }

    Serial.println("PM read unsuccessful after 3 retries");

    sensorValues.pm1_0 = NULL;
    sensorValues.pm2_5 = NULL;
    sensorValues.pm10_0 = NULL;
    sensorValues.aqiPM2_5 = NULL;
    sensorValues.aqiPM10_0 = NULL;
    sensorValues.pmsStatus = NOK;

    return false;
}

bool Sensors::updateBME() {
    if (millis() - lastBmeRead > BME_EVERY_MS) {
        bool success = bme680.readBME();

        if (success) {
            lastBmeRead = millis();

            sensorValues.temperature_bme680 = bme680.iaqSensor.temperature;
            sensorValues.pressure = bme680.iaqSensor.pressure / 100;
            sensorValues.pressureSeaLevel = calcPressureSeaLevel(sensorValues.pressure,
                                                                 sensorValues.temperature_bme680);
            sensorValues.humidity_bme680 = bme680.iaqSensor.humidity;

            if (bme680.iaqSensor.iaqAccuracy > 0) {
                sensorValues.aqiVoc = bme680.iaqSensor.staticIaq;
            } else {
                sensorValues.aqiVoc = NULL;
            }
            if (bme680.iaqSensor.breathVocAccuracy > 0) {
                sensorValues.voc = bme680.iaqSensor.breathVocEquivalent;
            } else {
                sensorValues.voc = NULL;
            }

            sensorValues.bmeStatus = OK;

            return true;
        } else {
            Serial.println("BME read failure");

            sensorValues.temperature_bme680 = NULL;
            sensorValues.pressure = NULL;
            sensorValues.pressureSeaLevel = NULL;
            sensorValues.humidity_bme680 = NULL;
            sensorValues.aqiVoc = NULL;
            sensorValues.voc = NULL;
            sensorValues.bmeStatus = NOK;
        }
    }

    return false;
}

bool Sensors::updateSHT() {
    if (millis() - lastSHTRead > SHT_EVERY_MS) {
        SHT::data data;
        bool success = sht.read(&data);
        if (success) {
            sensorValues.temperature_sht3x = data.temperature;
            sensorValues.humidity_sht3x = data.humidity;
            sensorValues.shtStatus = OK;
            lastSHTRead = millis();
            return true;
        } else {
            sensorValues.temperature_sht3x = NULL;
            sensorValues.humidity_sht3x = NULL;
            sensorValues.shtStatus = NOK;
        }
    }

    return false;
}

float Sensors::calcPressureSeaLevel(float pressure, float temp) {
    return pressure / pow((1. - 0.0065 * HEIGHT_ABOVE_SEA_LEVEL / (temp + 273.15 + 0.0065 * HEIGHT_ABOVE_SEA_LEVEL)),
                          (0.03416 / 0.0065));
}

void Sensors::updateAQIMax() {
    float aqiMax = NULL;

    aqiMax = max(sensorValues.aqiVoc, aqiMax);
    aqiMax = max(sensorValues.aqiCO2, aqiMax);
    aqiMax = max(sensorValues.aqiPM2_5, aqiMax);
    aqiMax = max(sensorValues.aqiPM10_0, aqiMax);

    sensorValues.aqiMax = aqiMax;
}

Sensors::SensorValues *Sensors::values() {
    return &sensorValues;
}

void Sensors::resetValues() {
    sensorValues = {
            .co2 = NULL,
            .pm1_0 = NULL,
            .pm2_5 = NULL,
            .pm10_0 = NULL,
            .temperature_bme680=NULL,
            .temperature_sht3x=NULL,
            .pressure=NULL,
            .pressureSeaLevel=NULL,
            .humidity_bme680=NULL,
            .humidity_sht3x=NULL,
            .voc=NULL,
            .aqiVoc=NULL,
            .aqiPM2_5=NULL,
            .aqiPM10_0=NULL,
            .aqiCO2=NULL,
            .co2Status = NOK,
            .bmeStatus = NOK,
            .pmsStatus = NOK,
            .shtStatus = NOK,
    };
}