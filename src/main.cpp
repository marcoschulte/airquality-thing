#include <Arduino.h>
#include "git_version.h"
#include "WifiMgr.h"

WifiMgr *wifiMgr;

void initWifiBlocking();

void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(10); }

    Serial.println("\n\nAirQuality device starting, rev " + String(GIT_HASH));

    initWifiBlocking();
}

void initWifiBlocking() {
    wifiMgr = new WifiMgr();
    wifiMgr->init();
}

void loop() {
    wifiMgr->tick();
}