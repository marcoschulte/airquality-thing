
#ifndef AIRQUALITY_ESP32_AIRQUALITITYSERVER_H
#define AIRQUALITY_ESP32_AIRQUALITITYSERVER_H

#include <ESP8266WebServer.h>
#include "Sensors.h"

class AirQualitityServer {
public:
    void start(Sensors *sensors);
    void tick();
};


#endif //AIRQUALITY_ESP32_AIRQUALITITYSERVER_H
