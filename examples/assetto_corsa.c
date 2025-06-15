#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdint.h>
#include <string.h>

#include "libracing/ac.h"

void receive_data(ac_client_t *client) {
    char *buffer = malloc(AC_BUFFER_SIZE);
    memset(buffer, 0, AC_BUFFER_SIZE);

    ac_event_t event;
    ac_event_type_t event_type;

    const ac_status_t received_status = ac_client_receive(client, buffer, &event, &event_type);
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
            fprintf(stdout, "\tSpeed: %f\n", event.car_info->speed_kmh);
            fprintf(stdout, "\tGas: %f\n", event.car_info->gas);
            fprintf(stdout, "\tBreak: %f\n", event.car_info->brake);
            fprintf(stdout, "\tClutch: %f\n", event.car_info->clutch);
            fprintf(stdout, "\tRPM: %f\n", event.car_info->engine_rpm);
            fprintf(stdout, "\tSteer: %f\n", event.car_info->steer);
            fprintf(stdout, "\tGear: %d\n", event.car_info->gear);
            fprintf(stdout, "\n");
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

    ac_client_t updater = ac_new_client();
    ac_client_t spotter = ac_new_client();

    const ac_status_t init_updater_status = ac_client_init(&updater);
    if (init_updater_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_init for updater failed with error %d\n", init_updater_status);
        return -1;
    }

    const ac_status_t init_spotter_status = ac_client_init(&spotter);
    if (init_spotter_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_init for spotter failed with error %d\n", init_spotter_status);
        return -1;
    }

    const ac_status_t handshake_updater_status = ac_client_handshake(&updater);
    if (handshake_updater_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_handshake for updater failed with error %d\n", handshake_updater_status);
        return -1;
    }

    const ac_status_t handshake_spotter_status = ac_client_handshake(&spotter);
    if (handshake_spotter_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_handshake for spotter failed with error %d\n", handshake_spotter_status);
        return -1;
    }

    receive_data(&updater);
    receive_data(&spotter);

    const ac_status_t subscribe_update_status = ac_client_subscribe_update(&updater);
    if (subscribe_update_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_subscribe_update failed with error %d\n", subscribe_update_status);
        return -1;
    }

    const ac_status_t subscribe_spot_status = ac_client_subscribe_spot(&spotter);
    if (subscribe_spot_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_subscribe_spot failed with error %d\n", subscribe_spot_status);
        return -1;
    }

    for (int i = 0; i < 10000; i++) {
        receive_data(&updater);
        receive_data(&spotter);
    }

    const ac_status_t dismiss_updater_status = ac_client_dismiss(&updater);
    if (dismiss_updater_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_dismiss for updater failed with error %d\n", dismiss_updater_status);
        return -1;
    }

    const ac_status_t dismiss_spotter_status = ac_client_dismiss(&spotter);
    if (dismiss_spotter_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_dismiss for spotter failed with error %d\n", dismiss_spotter_status);
        return -1;
    }

    const ac_status_t close_updater_status = ac_client_close(&updater);
    if (close_updater_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_close for updater failed with error %d\n", close_updater_status);
        return -1;
    }

    const ac_status_t close_spotter_status = ac_client_close(&spotter);
    if (close_spotter_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_close for spotter failed with error %d\n", close_spotter_status);
        return -1;
    }

    return 0;
}