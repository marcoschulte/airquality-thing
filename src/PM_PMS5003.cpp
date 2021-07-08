#include "PM_PMS5003.h"
#include <SoftwareSerial.h>

const uint8_t PM_PMS5003::command_active[] = {0x42, 0x4D, 0xE1, 0x00, 0x01, 0x01, 0x71};
const uint8_t PM_PMS5003::command_passive[] = {0x42, 0x4D, 0xE1, 0x00, 0x00, 0x01, 0x70};
const uint8_t PM_PMS5003::command_request_data[] = {0x42, 0x4D, 0xE2, 0x00, 0x00, 0x01, 0x71};
const uint8_t PM_PMS5003::command_sleep[] = {0x42, 0x4D, 0xE4, 0x00, 0x00, 0x01, 0x73};
const uint8_t PM_PMS5003::command_wake[] = {0x42, 0x4D, 0xE4, 0x00, 0x01, 0x01, 0x74};

void PM_PMS5003::init() {
    serial = new SoftwareSerial(rx, tx);
    serial->begin(9600);
    setModePassive();
}

void PM_PMS5003::setModeActive() {
    serial->write(command_active, sizeof command_active);
}

void PM_PMS5003::setModePassive() {
    serial->write(command_passive, sizeof command_passive);
}

void PM_PMS5003::sleep() {
    serial->write(command_sleep, sizeof command_sleep);
}

void PM_PMS5003::wake() {
    serial->write(command_wake, sizeof command_wake);
}

bool PM_PMS5003::read(PM_PMS5003::data *data) {
    // drop all received data
    serial->flush();

    // request new data
    serial->write(command_request_data, sizeof command_request_data);

    // look for magic byte marking beginning of data
    int peek;
    int waitingMs = 0;
    while ((peek = serial->peek()) != 0x42 || serial->available() < 32) {
        if (peek != 0x42 && peek != -1) {
            serial->read();

        } else {
            waitingMs += WAIT_INTERVAL;
            delay(WAIT_INTERVAL);

            if (waitingMs > TIMEOUT) {
                // timeout
                return false;
            }
        }
    }

    uint8_t buffer[32];
    serial->readBytes(buffer, 32);

    uint16_t checksum = 0;
    for (uint8_t i = 0; i < 30; i++) {
        checksum += buffer[i];
    }

    uint16_t buffer_u16[15];
    for (uint8_t i = 0; i < 15; i++) {
        buffer_u16[i] = buffer[2 + i * 2 + 1];
        buffer_u16[i] += (buffer[2 + i * 2] << 8);
    }

    memcpy((void *) data, (void *) buffer_u16, 30);

    if (checksum != data->checksum) {
        // checksum failure
        return false;
    }

    return true;
}




