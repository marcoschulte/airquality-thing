#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include "PM_PMS5003.h"

PM_PMS5003 pm = PM_PMS5003(D5, D6);

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    Serial.println("Hello");

    pm.init();
}

void loop() {
    struct PM_PMS5003::data data;

    // data after wake-up only available after second read and "enough" time to wake up.
    // otherwise particle count is 0 and pm ~ 2
    for (int i = 0; i < 2; i++) {
        if (pm.read(&data)) {
            Serial.println("concentration units");
            Serial.print("(std) PM 1.0: ");
            Serial.print(data.pm1_0_std);
            Serial.print("ug/m3\t\tPM 2.5: ");
            Serial.print(data.pm2_5_std);
            Serial.print("ug/m3\t\tPM 10: ");
            Serial.print(data.pm10_0_std);
            Serial.println("ug/m3");
            Serial.print("(env) PM 1.0: ");
            Serial.print(data.pm1_0_env);
            Serial.print("ug/m3\t\tPM 2.5: ");
            Serial.print(data.pm2_5_env);
            Serial.print("ug/m3\t\tPM 10: ");
            Serial.print(data.pm10_0_env);
            Serial.println("ug/m3");
            Serial.println();

            Serial.println("particles / 0.1L");
            Serial.print("> 0.3um: ");
            Serial.println(data.particles_0_3um);
            Serial.print("> 0.5um: ");
            Serial.println(data.particles_0_5um);
            Serial.print("> 1.0um: ");
            Serial.println(data.particles_1_0um);
            Serial.print("> 2.5um: ");
            Serial.println(data.particles_2_5um);
            Serial.print("> 5.0um: ");
            Serial.println(data.particles_5_0um);
            Serial.print("> 10.0um: ");
            Serial.println(data.particles_10_0um);
            Serial.println();
        } else {
            Serial.println("read failed");
        }
    }

    Serial.println("sleep");
    pm.sleep();
    delay(3000);

    Serial.println("wake up");
    Serial.println();
    pm.wake();
    delay(30000);
}
