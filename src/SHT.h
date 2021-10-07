
#ifndef AIRQUALITY_ESP32_SHT_H
#define AIRQUALITY_ESP32_SHT_H

#include <WEMOS_SHT3X.h>

class SHT {
    SHT3X sht3x = SHT3X(0x44);

public:
    struct data {
        float temperature, humidity;
    };

    bool read(data *);
};


#endif //AIRQUALITY_ESP32_SHT_H
