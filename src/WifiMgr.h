#ifndef AIRQUALITY_ESP32_WIFIMGR_H
#define AIRQUALITY_ESP32_WIFIMGR_H

#define USE_LITTLEFS            true
#define ESP_DRD_USE_LITTLEFS    true
#define DRD_TIMEOUT             2
#define DRD_ADDRESS             0

#include <ESP_DoubleResetDetector.h>
#include <WiFiManager.h>

class WifiMgr {
public:
    void init();
    void tick();
};


#endif //AIRQUALITY_ESP32_WIFIMGR_H
