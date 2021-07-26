
#ifndef AIRQUALITY_ESP32_AQICALCULATOR_H
#define AIRQUALITY_ESP32_AQICALCULATOR_H


class AQICalculator {
public:
    float calculateAQI(float value, const float thresholds[8]);

    const float thresholdsPM2_5[8] = {0, 12, 35.5, 55.5, 150.5, 250.5, 350.5, 500.5};
    const float thresholdsPM10_0[8] = {0, 55, 155, 255, 355, 425, 505, 605};
    const float thresholdsCO2[8] = {0, 800, 1000, 1500, 2000, 5000, 8000, 1200};

private:
    const int minLevels[8] = {0, 50, 100, 150, 200, 300, 400, 500};
};


#endif //AIRQUALITY_ESP32_AQICALCULATOR_H
