#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdint.h>
#include <string.h>

#include "libracing/ac.h"

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

    ac_event_t event;
    ac_event_type_t event_type;

    const int buffer_length = 408;
    char *buffer = malloc(buffer_length);
    memset(buffer, 0, buffer_length);

    const ac_status_t handshake_response_status = ac_receive(buffer, buffer_length, &event, &event_type);
    if (handshake_response_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_receive() failed with error %d\n", handshake_response_status);
        return -1;
    }

    fprintf(stdout, "Handshake response:\n");
    fprintf(stdout, "\tEventType: %d\n", event_type);
    fprintf(stdout, "\tEvent.Handshake.Identifier: %d\n", event.handshake->identifier);
    fprintf(stdout, "\tEvent.Handshake.Version: %d\n", event.handshake->version);
    wprintf(L"\tEvent.Handshake.CarName: %ls\n", event.handshake->car_name);
    wprintf(L"\tEvent.Handshake.DriverName: %ls\n", event.handshake->driver_name);
    wprintf(L"\tEvent.Handshake.TrackName: %ls\n", event.handshake->track_name);
    wprintf(L"\tEvent.Handshake.TrackConfig: %ls\n", event.handshake->track_config);
    fprintf(stdout, "\n");

    free(buffer);

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

