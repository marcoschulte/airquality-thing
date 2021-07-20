#ifndef AIRQUALITY_ESP32_WIFIMGR_H
#define AIRQUALITY_ESP32_WIFIMGR_H

#define USE_LITTLEFS            true
#define ESP_DRD_USE_LITTLEFS    true
#define DRD_TIMEOUT             2
#define DRD_ADDRESS             0

#include <ESP_DoubleResetDetector.h>
#include <WiFiManager.h>
#include <functional>

class WifiMgr {
public:
    void connect(std::function<void()>);

    void tick();

private:
    static std::function<void()> connectedCallback;
    WiFiManager wifiManager;
    DoubleResetDetector *drd;

    static void onConnected();
};

#endif //AIRQUALITY_ESP32_WIFIMGR_H
