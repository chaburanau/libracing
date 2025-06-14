#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdint.h>
#include <string.h>

#include "libracing/ac.h"

void receive_data() {
    const int buffer_length = 408;
    char *buffer = malloc(buffer_length);
    memset(buffer, 0, buffer_length);

    ac_event_t event;
    ac_event_type_t event_type;

    const ac_status_t received_status = ac_receive(buffer, buffer_length, &event, &event_type);
    if (received_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_receive() failed with error %d\n", received_status);
        return;
    }

    switch (event_type) {
        case AC_EVENT_TYPE_HANDSHAKE:
            fprintf(stdout, "\tIdentifier: %d\n", event.handshake->identifier);
            fprintf(stdout, "\tVersion: %d\n", event.handshake->version);
            wprintf(L"\tCarName: %ls\n", event.handshake->car_name);
            wprintf(L"\tDriverName: %ls\n", event.handshake->driver_name);
            wprintf(L"\tTrackName: %ls\n", event.handshake->track_name);
            wprintf(L"\tTrackConfig: %ls\n", event.handshake->track_config);
            fprintf(stdout, "\n");
            break;
        case AC_EVENT_TYPE_CAR_INFO:
            // fprintf(stdout, "\tSpeed: %f\n", event.car_info->speed_kmh);
            // fprintf(stdout, "\tGas: %f\n", event.car_info->gas);
            // fprintf(stdout, "\tBreak: %f\n", event.car_info->brake);
            // fprintf(stdout, "\tClutch: %f\n", event.car_info->clutch);
            // fprintf(stdout, "\tRPM: %f\n", event.car_info->engine_rpm);
            // fprintf(stdout, "\tSteer: %f\n", event.car_info->steer);
            // fprintf(stdout, "\tGear: %d\n", event.car_info->gear);
            // fprintf(stdout, "\n");
            break;
        case AC_EVENT_TYPE_LAP_INFO:
            fprintf(stdout, "\tCarNumber: %d\n", event.lap_info->car_number);
            fprintf(stdout, "\tLap: %d\n", event.lap_info->lap);
            fprintf(stdout, "\tTime: %d\n", event.lap_info->time);
            wprintf(L"\tCarName: %ls\n", event.lap_info->car_name);
            wprintf(L"\tDriverName: %ls\n", event.lap_info->driver_name);
            fprintf(stdout, "\n");
            break;
        default:
            fprintf(stderr, "Unknown event type: %d\n", event_type);
            break;
    }

    free(buffer);
}

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "en_US.UTF-8");

    const ac_handshaker_request_t handshake_request = {
        .identifier = AC_IDENTIFIER,
        .version = AC_SERVER_VERSION,
        .operation = AC_OPERATION_HANDSHAKE,
    };

    const ac_handshaker_request_t dismiss_request = {
        .identifier = AC_IDENTIFIER,
        .version = AC_SERVER_VERSION,
        .operation = AC_OPERATION_DISMISS,
    };

    const ac_handshaker_request_t subscribe_car_request = {
        .identifier = AC_IDENTIFIER,
        .version = AC_SERVER_VERSION,
        .operation = AC_OPERATION_SUBSCRIBE_UPDATE,
    };

    const ac_handshaker_request_t subscribe_lap_request = {
        .identifier = AC_IDENTIFIER,
        .version = AC_SERVER_VERSION,
        .operation = AC_OPERATION_SUBSCRIBE_SPOT,
    };

    const ac_status_t init_status = ac_init();
    if (init_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_init() failed with error %d\n", init_status);
        return -1;
    }

    const ac_status_t handshake_request_status = ac_send(handshake_request);
    if (handshake_request_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_send(handshake) failed with error %d\n", handshake_request_status);
        return -1;
    }

    receive_data();

    const ac_status_t subscribe_update_status = ac_send(subscribe_car_request);
    if (subscribe_update_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_send(subscribe_update) failed with error %d\n", subscribe_update_status);
        return -1;
    }

    receive_data();

    const ac_status_t subscribe_spot_status = ac_send(subscribe_lap_request);
    if (subscribe_spot_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_send(subscribe_spot) failed with error %d\n", subscribe_spot_status);
        return -1;
    }

    for (int i = 0; i < 100000000; i++) {
        receive_data();
    }

    const ac_status_t dismiss_request_status = ac_send(dismiss_request);
    if (dismiss_request_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_send(dismiss) failed with error %d\n", dismiss_request_status);
        return -1;
    }

    const ac_status_t close_status = ac_close();
    if (close_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_close() failed with error %d\n", close_status);
        return -1;
    }

    return 0;
}