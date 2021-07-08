
#include "CO2_S8.h"

void CO2_S8::init() {
    serial = new SoftwareSerial(rx, tx);
    serial->begin(9600);
}

int CO2_S8::read() {
    int retryLimit = 5;
    int retryCount = 0;

    int result = -1;
    while (result == -1) {
        if (retryCount == retryLimit) {
            break;
        }

        retryCount++;
        result = read_internal();
    }
    return result;
}

int CO2_S8::read_internal() {
    const byte readCommand[] = {0xFE, 0X44, 0X00, 0X08, 0X02, 0X9F, 0X25};

    int retry = 0;
    while (!serial->available()) {
        retry++;
        // keep sending request until we start to get a response
        serial->write(readCommand, sizeof readCommand);
        delay(50);
        if (retry > 10) {
            return -1;
        }
    }

    int timeout = 0;

    while (serial->available() < 7) {
        timeout++;
        if (timeout > 10) {
            while (serial->available()) {
                serial->read();
            }
            break;
        }
        delay(50);
    }

    byte buffer[] = {0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 7; i++) {
        int byte = serial->read();
        if (byte == -1) {
            return -1;
        }
        buffer[i] = byte;
    }
    int high = buffer[3];
    int low = buffer[4];
    unsigned long val = high * 256 + low;

    return val;
}