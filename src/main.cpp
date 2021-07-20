#include <Arduino.h>
#include "git_version.h"
#include "WifiMgr.h"
#include "Sensors.h"

WifiMgr *wifiMgr;
Sensors *sensors;

void initWifi();

void initSensors();

void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(10); }
    Serial.println("\n\nAirQuality device starting, rev #" + String(GIT_HASH));

    initWifi();
    initSensors();

}

void connected() {
    Serial.println("HOOOORAY Wifi connected");
}

void initWifi() {
    Serial.println("Initializing Wifi");
    wifiMgr = new WifiMgr();
    wifiMgr->connect(&connected);
}

void initSensors() {
    Serial.println("Initializing Sensors");
    sensors = new Sensors();
    sensors->init();
}

void loop() {
    wifiMgr->tick();
    sensors->tick();

    Serial.println("CO2 value " + String(sensors->values()->co2));
    Serial.println("PM values " + String(sensors->values()->pm1_0) + " " + String(sensors->values()->pm2_5) + " " +
                   String(sensors->values()->pm10_0));

    delay(2000);
}
