#include <Arduino.h>
#include "git_version.h"


void setup() {
    Serial.begin(115200);
    while (!Serial) { delay(10); }

    Serial.println("\n\nAirQuality device starting, rev " + String(GIT_HASH));
}

void loop() {

}