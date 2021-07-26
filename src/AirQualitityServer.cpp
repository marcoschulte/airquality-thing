
#include "AirQualitityServer.h"

#define HTTP_SERVER_PORT 80
#define DEVICE_NAME "airqualitything1"

ESP8266WebServer http_server = ESP8266WebServer(HTTP_SERVER_PORT);
Sensors *staticSensors;

char *commentOnError(Sensors::sensor_status_t status) {
    if (status == Sensors::OK) {
        return "";
    } else {
        return "# ";
    }
}

void handlePrometheusClient() {
    static size_t const BUFSIZE = 1024;
    static char const *response_template =
            "# TYPE temperature_celsius gauge\n"
            "%stemperature_celsius{device=\"%s\"} %f\n"
            "# TYPE pressure_hpa gauge\n"
            "%spressure_hpa{device=\"%s\"} %f\n"
            "# TYPE pressure_sl_hpa gauge\n"
            "%spressure_sl_hpa{device=\"%s\"} %f\n"
            "# TYPE humidity_percent gauge\n"
            "%shumidity_percent{device=\"%s\"} %f\n"
            "# TYPE static_iaq gauge\n"
            "%sstatic_iaq{device=\"%s\"} %f\n"
            "# TYPE voc_ppm gauge\n"
            "%svoc_ppm{device=\"%s\"} %f\n"
            "# TYPE co2_ppm gauge\n"
            "%sco2_ppm{device=\"%s\"} %i\n"
            "# TYPE pm1_0_ug_m3 gauge\n"
            "%spm1_0_ug_m3{device=\"%s\"} %i\n"
            "# TYPE pm2_5_ug_m3 gauge\n"
            "%spm2_5_ug_m3{device=\"%s\"} %i\n"
            "# TYPE pm10_0_ug_m3 gauge\n"
            "%spm10_0_ug_m3{device=\"%s\"} %i\n";


    char response[BUFSIZE];
    Sensors::SensorValues *values = staticSensors->values();
    snprintf(response, BUFSIZE, response_template,
             commentOnError(values->bmeStatus), DEVICE_NAME, values->temperature,
             commentOnError(values->bmeStatus), DEVICE_NAME, values->pressure,
             commentOnError(values->bmeStatus), DEVICE_NAME, values->pressureSeaLevel,
             commentOnError(values->bmeStatus), DEVICE_NAME, values->humidity,
             commentOnError(values->bmeStatus), DEVICE_NAME, values->staticIaq,
             commentOnError(values->bmeStatus), DEVICE_NAME, values->breathVocEquivalent,
             commentOnError(values->co2Status), DEVICE_NAME, values->co2,
             commentOnError(values->pmsStatus), DEVICE_NAME, values->pm1_0,
             commentOnError(values->pmsStatus), DEVICE_NAME, values->pm2_5,
             commentOnError(values->pmsStatus), DEVICE_NAME, values->pm10_0
    );
    http_server.send(200, "text/plain; charset=utf-8", response);
}

void AirQualitityServer::start(Sensors *sensors) {
    staticSensors = sensors;

    http_server.on("/prometheus", HTTPMethod::HTTP_GET, handlePrometheusClient);
    http_server.begin();
}

void AirQualitityServer::tick() {
    http_server.handleClient();
}