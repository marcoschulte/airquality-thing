#include "Display.h"

#define FRAME_SWITCH_MS 2 * 1000

void Display::init() {
    display.init();
}

void Display::tick() {
    if (millis() - lastFrame > FRAME_SWITCH_MS) {
        if (frame == 0) {
            displayFrame1();
        } else if (frame == 1) {
            displayFrame2();
        }

        frame = (frame + 1) % 2;
        lastFrame = millis();
    }
}

void Display::displayFrame1() {
    Sensors::SensorValues *sensorValues = sensors->values();

    display.clear();
    display.setFont(ArialMT_Plain_10);

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0 + 32, 0, String((int) (sensorValues->temperature + 0.5f)) + "°");
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    display.drawString(64 + 32, 0, String((int) (sensorValues->humidity + 0.5f)) + "%");

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0 + 32, 12, String(sensorValues->pressure) + "hPa");

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0 + 32, 24, "VOC " + String(sensorValues->voc) + "ppm");

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0 + 32, 36, "CO2 " + String(sensorValues->co2) + "ppm");
    display.display();
}

void Display::displayFrame2() {
    Sensors::SensorValues *sensorValues = sensors->values();

    display.clear();
    display.setFont(ArialMT_Plain_10);

    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0 + 32, 0, "IAQ " + String((int) (sensorValues->aqiMax + 0.5f)));
    display.drawString(0 + 32, 12, "PM 1.0 " + String(sensorValues->pm1_0) + "µg/m3");
    display.drawString(0 + 32, 24, "PM 2.5 " + String(sensorValues->pm2_5) + "µg/m3");
    display.drawString(0 + 32, 36, "PM 10 " + String(sensorValues->pm10_0) + "µg/m3");
    display.display();
}