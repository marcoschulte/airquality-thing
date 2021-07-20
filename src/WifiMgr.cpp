#include "WifiMgr.h"

const char *apName = "AirQuality Device";

WiFiManager wifiManager;
DoubleResetDetector *drd;

void WifiMgr::init() {
    bool reset = false;
    drd = new DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS);
    if (drd->detectDoubleReset()) {
        Serial.println("Reset wifi, starting AP");
        reset = true;
    }

    boolean connected;
    if (reset) {
        connected = wifiManager.startConfigPortal(apName);
    } else {
        connected = wifiManager.autoConnect(apName);
    }

    if (connected) {
        Serial.println("Connected to wifi " + WiFi.SSID());
    } else {
        Serial.println("No wifi connection, sleep 30s then reboot");
        delay(30 * 1000);
        ESP.restart();
    }
}

void WifiMgr::tick() {
    drd->loop();
}