#ifndef AIRQUALITY_ESP32_PM_PMS5003_H
#define AIRQUALITY_ESP32_PM_PMS5003_H


static const int TIMEOUT = 2000;
static const int WAIT_INTERVAL = 50;

#include <cstdint>
#include <SoftwareSerial.h>


class PM_PMS5003 {
    int8_t rx, tx;
    SoftwareSerial *serial;
    static const uint8_t command_active[];
    static const uint8_t command_passive[];
    static const uint8_t command_request_data[];
    static const uint8_t command_sleep[];
    static const uint8_t command_wake[];

public:

    struct data {
        uint16_t framelen,
                pm1_0_std,
                pm2_5_std,
                pm10_0_std,
                pm1_0_env,
                pm2_5_env,
                pm10_0_env,
                particles_0_3um,
                particles_0_5um,
                particles_1_0um,
                particles_2_5um,
                particles_5_0um,
                particles_10_0um,
                reserved,
                checksum;
    };

    PM_PMS5003(int rx, int tx) : rx(rx), tx(tx) {};

    void init();

    void setModeActive();

    void setModePassive();

    void sleep();

    void wake();

    bool read(data *);

};


#endif //AIRQUALITY_ESP32_PM_PMS5003_H
