
#ifndef AIRQUALITY_ESP32_BME680_H
#define AIRQUALITY_ESP32_BME680_H

#include <bsec.h>
#include <Wire.h>

const uint8_t bsec_config_iaq[] = {
#include "config/generic_33v_3s_4d/bsec_iaq.txt"
};

class BME680 {
public:
    Bsec iaqSensor;

    void init();

    bool readBME();

private:
    void checkIaqSensorStatus(void);
};


#endif //AIRQUALITY_ESP32_BME680_H
