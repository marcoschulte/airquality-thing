
#include "SHT.h"

bool SHT::read(SHT::data *data) {
    if (sht3x.get() == 0) {
        data->temperature = sht3x.cTemp;
        data->humidity = sht3x.humidity;
        return true;
    }

    return false;
}