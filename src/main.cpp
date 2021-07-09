#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <SPI.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include "PM_PMS5003.h"
#include "CO2_S8.h"
#include "bsec.h"
#include <FastLED.h>
#include "SSD1306Wire.h"

#define LED_PIN     D7
#define NUM_LEDS    1
CRGB leds[NUM_LEDS];

#define SEALEVELPRESSURE_HPA (1013.25)

PM_PMS5003 pm = PM_PMS5003(D5, D6);
CO2_S8 co2 = CO2_S8(D4, D3);
Bsec iaqSensor;
SSD1306Wire display(0x3c, SDA, SCL);

String output;

void readPM();

void errLeds(void) {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
}

void displayTextDemo() {
    while (true) {
        for (int x = 0; x < 64; x++) {
            for (int y = 0; y < 48; y++) {
                display.clear();
                display.setTextAlignment(TEXT_ALIGN_LEFT);
                display.setFont(ArialMT_Plain_10);
                display.drawString(x + 32, y, String(x + 32) + "," + String(y));
                display.display();
                delay(15);
            }
        }
    }
}

// Helper function definitions
void checkIaqSensorStatus(void) {
    if (iaqSensor.status != BSEC_OK) {
        if (iaqSensor.status < BSEC_OK) {
            output = "BSEC error code : " + String(iaqSensor.status);
            Serial.println(output);
            for (;;)
                errLeds(); /* Halt in case of failure */
        } else {
            output = "BSEC warning code : " + String(iaqSensor.status);
            Serial.println(output);
        }
    }

    if (iaqSensor.bme680Status != BME680_OK) {
        if (iaqSensor.bme680Status < BME680_OK) {
            output = "BME680 error code : " + String(iaqSensor.bme680Status);
            Serial.println(output);
            for (;;)
                errLeds(); /* Halt in case of failure */
        } else {
            output = "BME680 warning code : " + String(iaqSensor.bme680Status);
            Serial.println(output);
        }
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(10); };
    Wire.begin();
    Serial.println("Hello");

    FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(50);

    display.init();

    iaqSensor.begin(BME680_I2C_ADDR_SECONDARY, Wire);
    output = "\nBSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) +
             "." +
             String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix);
    Serial.println(output);
    checkIaqSensorStatus();

    bsec_virtual_sensor_t sensorList[10] = {
            BSEC_OUTPUT_RAW_PRESSURE,
            BSEC_OUTPUT_STATIC_IAQ,
            BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
            BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
            BSEC_OUTPUT_CO2_EQUIVALENT,
            BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
    };

    iaqSensor.updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);
    checkIaqSensorStatus();

    pm.init();
    co2.init();
}

void readBME() {
    unsigned long time_trigger = millis();
    if (iaqSensor.run()) { // If new data is available
        output = "Timestamp [ms], pressure [hPa], IAQ, temperature [°C], relative humidity [%], CO2 equivalent, breath VOC equivalent";
        Serial.println(output);

        output = String(time_trigger);
        output += ", " + String(iaqSensor.pressure / 100) + "hPa";
        output += ", " + String(iaqSensor.staticIaq);
        output += ", " + String(iaqSensor.temperature) + "°C";
        output += ", " + String(iaqSensor.humidity) + "%";
        output += ", " + String(iaqSensor.co2Equivalent) + "ppm";
        output += ", " + String(iaqSensor.breathVocEquivalent) + "ppm";
        Serial.println(output);
    } else {
        checkIaqSensorStatus();
    }
}

void readCO2() {
    int co2Result = co2.read();
    Serial.print("CO2: ");
    Serial.print(co2Result);
    Serial.println("ppm");

    display.clear();
    display.setFont(ArialMT_Plain_10);

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0 + 32, 0, String((int) (iaqSensor.temperature + 0.5f)) + "°");
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(64 + 32, 0, String((int) (iaqSensor.humidity + 0.5f)) + "%");

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0 + 32, 12, String(iaqSensor.pressure / 100) + "hPa");

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0 + 32, 24, "VOC " + String(iaqSensor.breathVocEquivalent) + "ppm");

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0 + 32, 36, "CO2 " + String(co2Result) + "ppm");
    display.display();
}

void readPM() {
    struct PM_PMS5003::data data;

    // data after wake-up only available after second read and "enough" time to wake up.
// otherwise particle count is 0 and pm ~ 2
    for (int i = 0; i < 2; i++) {
        if (pm.read(&data)) {
            if (data.particles_0_3um == 0) {
                continue;
            }
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

            display.clear();
            display.setFont(ArialMT_Plain_10);
            display.setTextAlignment(TEXT_ALIGN_LEFT);
            display.drawString(0 + 32, 0, "PM 1.0 " + String(data.pm1_0_std) + "µg/m3");
            display.drawString(0 + 32, 12, "PM 2.5 " + String(data.pm2_5_std) + "µg/m3");
            display.drawString(0 + 32, 24, "PM 10 " + String(data.pm10_0_std) + "µg/m3");
            display.display();
            break;
        } else {
            Serial.println("read failed");
        }
    }

    /*
    Serial.println("sleep");
    pm.sleep();
    delay(3000);

    Serial.println("wake up");
    Serial.println();
    pm.wake();
    delay(30000);
     */
}

void loop() {
    leds[0] = CRGB::Red;
    FastLED.show();
    readBME();
//    delay(1000);

    leds[0] = CRGB::Green;
    FastLED.show();
    readCO2();
    delay(4000);

    leds[0] = CRGB::Blue;
    FastLED.show();
    readPM();
    delay(4000);
}

