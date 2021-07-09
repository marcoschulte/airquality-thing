#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <SPI.h>
#include <DNSServer.h>
#include <WiFiManager.h>
#include "PM_PMS5003.h"
#include "CO2_S8.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include "bsec.h"

#define SEALEVELPRESSURE_HPA (1013.25)

PM_PMS5003 pm = PM_PMS5003(D5, D6);
CO2_S8 co2 = CO2_S8(D4, D3);
//Adafruit_BME680 bme;
Bsec iaqSensor;
String output;

void readPM();

void errLeds(void) {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
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

    iaqSensor.begin(BME680_I2C_ADDR_SECONDARY, Wire);
    output = "\nBSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." +
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

    // Print the header
    output = "Timestamp [ms], pressure [hPa], IAQ, temperature [°C], relative humidity [%], CO2 equivalent, breath VOC equivalent";
    Serial.println(output);

    pm.init();
    co2.init();
}

void readBME() {
    unsigned long time_trigger = millis();
    if (iaqSensor.run()) { // If new data is available
        output = String(time_trigger);
        output += ", " + String(iaqSensor.pressure / 100);
        output += ", " + String(iaqSensor.staticIaq);
        output += ", " + String(iaqSensor.temperature);
        output += ", " + String(iaqSensor.humidity);
        output += ", " + String(iaqSensor.co2Equivalent);
        output += ", " + String(iaqSensor.breathVocEquivalent);
        Serial.println(output);
    } else {
        checkIaqSensorStatus();
    }
}

void readCO2() {
    int co2Result = co2.read();
    Serial.print("CO2: ");
    Serial.println(co2Result);
    delay(1000);
}

void readPM() {
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

void loop() {
    readBME();
    delay(2000);
}
