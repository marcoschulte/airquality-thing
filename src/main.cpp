#include <Arduino.h>
#include "git_version.h"
#include "WifiMgr.h"
#include "Sensors.h"
#include "Display.h"

WifiMgr wifiMgr;
Sensors sensors;
Display display(sensors.values());

void initWifi();

void initSensors();

void initDisplay();

void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(10); }
    Serial.println("\n\nAirQuality device starting, rev #" + String(GIT_HASH));

    Wire.begin();

    initWifi();
    initDisplay();
    display.tick();
    initSensors();
}

void connected() {
    Serial.println("HOOOORAY Wifi connected");
}

void initWifi() {
    Serial.println("Initializing Wifi");
    wifiMgr.connect(&connected);
}

void initSensors() {
    Serial.println("Initializing Sensors");
    sensors.init();
}

void initDisplay() {
    display.init();
}

void loop() {
    wifiMgr.tick();
    bool updated = sensors.tick();
    display.tick();

    if (updated) {
        Serial.println("CO2 value " + String(sensors.values()->co2));
        Serial.println("PM values " + String(sensors.values()->pm1_0)
                       + " " + String(sensors.values()->pm2_5)
                       + " " + String(sensors.values()->pm10_0));
        Serial.println("IAQ " + String(sensors.values()->staticIaq)
                       + " VOC: " + String(sensors.values()->breathVocEquivalent)
                       + " temp.: " + String(sensors.values()->temperature)
                       + " humidity: " + String(sensors.values()->humidity)
                       + " pressure: " + String(sensors.values()->pressure));
    }
}
