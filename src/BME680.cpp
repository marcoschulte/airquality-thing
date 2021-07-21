
#include "BME680.h"

void BME680::init() {
    iaqSensor.setTemperatureOffset(4);
    iaqSensor.begin(BME680_I2C_ADDR_SECONDARY, Wire);

    checkIaqSensorStatus();
    iaqSensor.setConfig(bsec_config_iaq);

    bsec_virtual_sensor_t sensorList[] = {
            BSEC_OUTPUT_RAW_TEMPERATURE,
            BSEC_OUTPUT_RAW_PRESSURE,
            BSEC_OUTPUT_RAW_HUMIDITY,
            BSEC_OUTPUT_RAW_GAS,
            BSEC_OUTPUT_IAQ,
            BSEC_OUTPUT_STATIC_IAQ,
            BSEC_OUTPUT_CO2_EQUIVALENT,
            BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
            BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
            BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
    };
iaqSensor.
    iaqSensor.updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);
    checkIaqSensorStatus();
}

void BME680::checkIaqSensorStatus(void) {
    if (iaqSensor.status != BSEC_OK) {
        if (iaqSensor.status < BSEC_OK) {
            Serial.println("BSEC error code : " + String(iaqSensor.status));
        } else {
            Serial.println("BSEC warning code : " + String(iaqSensor.status));
        }
    }

    if (iaqSensor.bme680Status != BME680_OK) {
        if (iaqSensor.bme680Status < BME680_OK) {
            Serial.println("BME680 error code : " + String(iaqSensor.bme680Status));
        } else {
            Serial.println("BME680 warning code : " + String(iaqSensor.bme680Status));
        }
    }
}

bool BME680::readBME() {
    if (iaqSensor.run()) {
        return true;
    } else {
        checkIaqSensorStatus();
        return false;
    }
}
