
#include "AirQualitityServer.h"

#define HTTP_SERVER_PORT 80
#define DEVICE_NAME "airthing1"

ESP8266WebServer http_server = ESP8266WebServer(HTTP_SERVER_PORT);
Sensors *staticSensors;

char *commentOnNull(float value) {
    if (value == NULL) {
        return "# ";
    } else {
        return "";
    }
}

void handlePrometheusClient() {
    static size_t const BUFSIZE = 1200;
    static char const *response_template =
            "# TYPE temperature_celsius gauge\n"
            "%stemperature_celsius{device=\"%s\"} %f\n"
            "# TYPE pressure_hpa gauge\n"
            "%spressure_hpa{device=\"%s\"} %f\n"
            "# TYPE pressure_sl_hpa gauge\n"
            "%spressure_sl_hpa{device=\"%s\"} %f\n"
            "# TYPE humidity_percent gauge\n"
            "%shumidity_percent{device=\"%s\"} %f\n"
            "# TYPE iaq_voc gauge\n"
            "%siaq_voc{device=\"%s\"} %f\n"
            "# TYPE iaq_co2 gauge\n"
            "%siaq_co2{device=\"%s\"} %f\n"
            "# TYPE iaq_pm2_5 gauge\n"
            "%siaq_pm2_5{device=\"%s\"} %f\n"
            "# TYPE iaq_pm10_0 gauge\n"
            "%siaq_pm10_0{device=\"%s\"} %f\n"
            "# TYPE iaq_max gauge\n"
            "%siaq_max{device=\"%s\"} %f\n"
            "# TYPE voc_ppm gauge\n"
            "%svoc_ppm{device=\"%s\"} %f\n"
            "# TYPE co2_ppm gauge\n"
            "%sco2_ppm{device=\"%s\"} %i\n"
            "# TYPE pm1_0_ug_m3 gauge\n"
            "%spm1_0_ug_m3{device=\"%s\"} %i\n"
            "# TYPE pm2_5_ug_m3 gauge\n"
            "%spm2_5_ug_m3{device=\"%s\"} %i\n"
            "# TYPE pm10_0_ug_m3 gauge\n"
            "%spm10_0_ug_m3{device=\"%s\"} %i\n"
            "# TYPE temperature_bme_celsius gauge\n"
            "%stemperature_bme_celsius{device=\"%s\"} %f\n"
            "# TYPE humidity_bme_percent gauge\n"
            "%shumidity_bme_percent{device=\"%s\"} %f\n";

    char response[BUFSIZE];
    Sensors::SensorValues *values = staticSensors->values();
    snprintf(response, BUFSIZE, response_template,
             commentOnNull(values->temperature_sht3x), DEVICE_NAME, values->temperature_sht3x,
             commentOnNull(values->pressure), DEVICE_NAME, values->pressure,
             commentOnNull(values->pressureSeaLevel), DEVICE_NAME, values->pressureSeaLevel,
             commentOnNull(values->humidity_sht3x), DEVICE_NAME, values->humidity_sht3x,
             commentOnNull(values->aqiVoc), DEVICE_NAME, values->aqiVoc,
             commentOnNull(values->aqiCO2), DEVICE_NAME, values->aqiCO2,
             commentOnNull(values->aqiPM2_5), DEVICE_NAME, values->aqiPM2_5,
             commentOnNull(values->aqiPM10_0), DEVICE_NAME, values->aqiPM10_0,
             commentOnNull(values->aqiMax), DEVICE_NAME, values->aqiMax,
             commentOnNull(values->voc), DEVICE_NAME, values->voc,
             commentOnNull(values->co2), DEVICE_NAME, values->co2,
             commentOnNull(values->pm1_0), DEVICE_NAME, values->pm1_0,
             commentOnNull(values->pm2_5), DEVICE_NAME, values->pm2_5,
             commentOnNull(values->pm10_0), DEVICE_NAME, values->pm10_0,
             commentOnNull(values->temperature_bme680), DEVICE_NAME, values->temperature_bme680,
             commentOnNull(values->humidity_bme680), DEVICE_NAME, values->humidity_bme680
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