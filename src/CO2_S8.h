
#ifndef AIRQUALITY_ESP32_CO2_S8_H
#define AIRQUALITY_ESP32_CO2_S8_H


#include <cstdint>
#include <SoftwareSerial.h>

class CO2_S8 {
    int8_t rx, tx;
    SoftwareSerial *serial;

public:
    CO2_S8(int rx, int tx) : rx(rx), tx(tx) {};

    void init();

    int read();

private:
    int read_internal();
};


#endif //AIRQUALITY_ESP32_CO2_S8_H
