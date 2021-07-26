#include "Sensors.h"

#define HEIGHT_ABOVE_SEA_LEVEL 525

#define CO2_EVERY_MS 5 * 1000

#define PM_EVERY_MS 30 * 1000
#define PM_WARMUP_MS 30 * 1000

#define BME_EVERY_MS 3 * 1000

void Sensors::init() {
    resetValues();
    co2S8.init();
    pm.init();
    pm.setModePassive();
    bme680.init();
}

bool Sensors::tick() {
    bool updated = false;

    updated = updateCO2() || updated;
    updated = updatePM() || updated;
    updated = updateBME() || updated;

    return updated;
}

bool Sensors::updateCO2() {
    if (millis() - lastCO2Read > CO2_EVERY_MS) {
        int co2 = co2S8.read();
        if (co2 == -1) {
            Serial.println("S8 read failure");
            sensorValues.co2 = NULL;
            sensorValues.co2Status = NOK;
        } else {
            sensorValues.co2 = co2;
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
    sensorValues.pmsStatus = NOK;

    return false;
}

bool Sensors::updateBME() {
    if (millis() - lastBmeRead > BME_EVERY_MS) {
        bool success = bme680.readBME();

        if (success) {
            lastBmeRead = millis();

            sensorValues.temperature = bme680.iaqSensor.temperature;
            sensorValues.pressure = bme680.iaqSensor.pressure / 100;
            sensorValues.pressureSeaLevel = calcPressureSeaLevel(sensorValues.pressure, sensorValues.temperature);
            sensorValues.humidity = bme680.iaqSensor.humidity;

            if (bme680.iaqSensor.iaqAccuracy > 0) {
                sensorValues.staticIaq = bme680.iaqSensor.staticIaq;
            } else {
                sensorValues.staticIaq = NULL;
            }
            if (bme680.iaqSensor.breathVocAccuracy > 0) {
                sensorValues.breathVocEquivalent = bme680.iaqSensor.breathVocEquivalent;
            } else {
                sensorValues.breathVocEquivalent = NULL;
            }

            sensorValues.bmeStatus = OK;

            return true;
        } else {
            Serial.println("BME read failure");

            sensorValues.temperature = NULL;
            sensorValues.pressure = NULL;
            sensorValues.pressureSeaLevel = NULL;
            sensorValues.humidity = NULL;
            sensorValues.staticIaq = NULL;
            sensorValues.breathVocEquivalent = NULL;
            sensorValues.bmeStatus = NOK;
        }
    }

    return false;
}

float Sensors::calcPressureSeaLevel(float pressure, float temp) {
    return pressure / pow((1. - 0.0065 * HEIGHT_ABOVE_SEA_LEVEL / (temp + 273.15 + 0.0065 * HEIGHT_ABOVE_SEA_LEVEL)),
                          (0.03416 / 0.0065));
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
            .temperature=NULL,
            .pressure=NULL,
            .pressureSeaLevel=NULL,
            .humidity=NULL,
            .staticIaq=NULL,
            .breathVocEquivalent=NULL,
            .co2Status = NOK,
            .bmeStatus = NOK,
            .pmsStatus = NOK,
    };
}