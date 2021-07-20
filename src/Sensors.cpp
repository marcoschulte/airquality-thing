#include "Sensors.h"

#define PM_EVERY_MS 30 * 1000
#define PM_WARMUP_MS 30 * 1000

void Sensors::init() {
    resetValues();
    co2S8.init();
    pm.init();
    pm.setModePassive();
}

void Sensors::tick() {
    updateCO2();
    updatePM();
}

void Sensors::updateCO2() {
    int co2 = co2S8.read();
    if (co2 == -1) {
        sensorValues.co2 = NULL;
    } else {
        sensorValues.co2 = co2;
    }
}

void Sensors::updatePM() {
    if (pmEnabled) {
        if (millis() - pmWarmupStartedAt > PM_WARMUP_MS) {
            Serial.println("PM read");

            doReadPM();

            Serial.println("PM sleep");
            pm.sleep();
            pmEnabled = false;
            lastPmRead = millis();
        }
    } else {
        if (millis() - lastPmRead > PM_EVERY_MS) {
            Serial.println("PM warm-up");
            pm.wake();
            pmEnabled = true;
            pmWarmupStartedAt = millis();
        }
    }
}

void Sensors::doReadPM() {
    PM_PMS5003::data data;
    for (int i = 0; i < 3; i++) {
        if (pm.read(&data)) {
            if (data.particles_0_3um == 0) {
                continue;
            }

            sensorValues.pm1_0 = data.pm1_0_std;
            sensorValues.pm2_5 = data.pm2_5_std;
            sensorValues.pm10_0 = data.pm10_0_std;

            return;
        } else {
            Serial.println("PM read failure");
        }
    }

    Serial.println("PM read unsuccessful after 3 retries");
}

Sensors::SensorValues *Sensors::values() {
    return &sensorValues;
}

void Sensors::resetValues() {
    sensorValues = {
            .co2 = NULL,
            .pm1_0 = NULL,
            .pm2_5 = NULL,
            .pm10_0 = NULL
    };
}