#include "WifiMgr.h"

const char *apName = "AirQualityThing";
std::function<void()> WifiMgr::connectedCallback = NULL;

void WifiMgr::connect(std::function<void()> _connectedCallback) {
    connectedCallback = _connectedCallback;

    wifiManager.setHostname(apName);

    wifiManager.setConfigPortalBlocking(false);
    wifiManager.setSaveConfigCallback(&onConnected);

    boolean connected;
    drd = new DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS);
    if (drd->detectDoubleReset()) {
        Serial.println("Reset Wifi");
        connected = wifiManager.startConfigPortal(apName);
    } else {
        connected = wifiManager.autoConnect(apName);
    }

    if (connected) {
        onConnected();
    }
}

void WifiMgr::onConnected() {
    Serial.print("Connected to Wifi " + WiFi.SSID() + " with hostname " + WiFi.hostname() + " and IP ");
    Serial.println(WiFi.localIP());
    WifiMgr::connectedCallback();
}

void WifiMgr::tick() {
    wifiManager.process();
    drd->loop();
}
