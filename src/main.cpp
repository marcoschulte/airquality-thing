#include <Arduino.h>
#include "git_version.h"
#include "WifiMgr.h"
#include "Sensors.h"
#include "NightMode.h"
#include "Display.h"
#include "AirQualitityServer.h"
#include "StatusLed.h"


WifiMgr wifiMgr;
Sensors sensors;
NightMode nightMode;
Display display(&sensors, &nightMode);
AirQualitityServer server;
StatusLed statusLed(&sensors, &nightMode);

void initWifi();

void initSensors();

void initDisplay();


void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(10); }
    Serial.println("\n\nAirQuality device starting, rev #" + String(GIT_HASH));

    statusLed.init();

    Wire.begin();

    initWifi();
    initDisplay();
    display.tick();
    initSensors();
}

void connected() {
    Serial.println("WiFi connected. Starting server");
    server.start(&sensors);

    nightMode.init();
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
    Serial.println("Initializing Display");
    display.init();
}

void loop() {
    wifiMgr.tick();
    bool updated = sensors.tick();
    display.tick();
    server.tick();
    nightMode.tick();

    if (updated) {
        Serial.print("CO2 " + String(sensors.values()->co2));
        Serial.print(", PM " + String(sensors.values()->pm1_0)
                     + " " + String(sensors.values()->pm2_5)
                     + " " + String(sensors.values()->pm10_0));
        Serial.println(", AQI VOC " + String(sensors.values()->aqiVoc)
                       + ", AQI CO2 " + String(sensors.values()->aqiCO2)
                       + ", AQI PM 2.5 " + String(sensors.values()->aqiPM2_5)
                       + ", AQI PM 10.0 " + String(sensors.values()->aqiPM10_0)
                       + ", AQI Max " + String(sensors.values()->aqiMax)
                       + ", VOC: " + String(sensors.values()->voc)
                       + ", temp. bme: " + String(sensors.values()->temperature_bme680)
                       + ", temp. sht: " + String(sensors.values()->temperature_sht3x)
                       + ", humidity bme: " + String(sensors.values()->humidity_bme680)
                       + ", humidity sht: " + String(sensors.values()->humidity_sht3x)
                       + ", pressure: " + String(sensors.values()->pressure)
                       + ", pressure SL: " + String(sensors.values()->pressureSeaLevel));
    }
}
