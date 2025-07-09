#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdint.h>

#include "../include/libracing/ac.h"

void receive_data(ac_client_t *client) {
    ac_event_t event;

    const ac_result_t received_status = ac_client_receive(client, &event);
    if (received_status.status != AC_STATUS_OK) {
        fprintf(stderr, "ac_receive() failed with error %d; %d\n", received_status.status, received_status.underlying_error);
        return;
    }

    switch (event.type) {
        case AC_EVENT_TYPE_HANDSHAKE:
            fprintf(stdout, "\tIdentifier: %d\n", event.data.handshake->identifier);
            fprintf(stdout, "\tVersion: %d\n", event.data.handshake->version);
            wprintf(L"\tCarName: %ls\n", event.data.handshake->car_name);
            wprintf(L"\tDriverName: %ls\n", event.data.handshake->driver_name);
            wprintf(L"\tTrackName: %ls\n", event.data.handshake->track_name);
            wprintf(L"\tTrackConfig: %ls\n", event.data.handshake->track_config);
            fprintf(stdout, "\n");
            free(event.data.handshake);
            break;
        case AC_EVENT_TYPE_CAR_INFO:
            fprintf(stdout, "\tSpeed: %f\n", event.data.car_info->speed_kmh);
            fprintf(stdout, "\tGas: %f\n", event.data.car_info->gas);
            fprintf(stdout, "\tBreak: %f\n", event.data.car_info->brake);
            fprintf(stdout, "\tClutch: %f\n", event.data.car_info->clutch);
            fprintf(stdout, "\tRPM: %f\n", event.data.car_info->engine_rpm);
            fprintf(stdout, "\tSteer: %f\n", event.data.car_info->steer);
            fprintf(stdout, "\tGear: %d\n", event.data.car_info->gear);
            fprintf(stdout, "\n");
            free(event.data.car_info);
            break;
        case AC_EVENT_TYPE_LAP_INFO:
            fprintf(stdout, "\tCarNumber: %d\n", event.data.lap_info->car_number);
            fprintf(stdout, "\tLap: %d\n", event.data.lap_info->lap);
            fprintf(stdout, "\tTime: %d\n", event.data.lap_info->time);
            wprintf(L"\tCarName: %ls\n", event.data.lap_info->car_name);
            wprintf(L"\tDriverName: %ls\n", event.data.lap_info->driver_name);
            fprintf(stdout, "\n");
            free(event.data.lap_info);
            break;
        default:
            fprintf(stderr, "Unknown event type: %d\n", event.type);
            break;
    }
}

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "en_US.UTF-8");

    ac_client_t* updater = malloc(sizeof(ac_client_t));
    ac_client_t* spotter = malloc(sizeof(ac_client_t));

    const ac_result_t handshake_updater_result = ac_client_handshake(updater);
    if (handshake_updater_result.status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_handshake for updater failed with error %d; %d\n",
                handshake_updater_result.status, handshake_updater_result.underlying_error);
        return -1;
    }

    const ac_result_t handshake_spotter_result = ac_client_handshake(spotter);
    if (handshake_spotter_result.status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_handshake for spotter failed with error %d; %d\n", 
                handshake_spotter_result.status, handshake_spotter_result.underlying_error);
        return -1;
    }

    receive_data(updater);
    receive_data(spotter);

    const ac_result_t subscribe_update_result = ac_client_subscribe_update(updater);
    if (subscribe_update_result.status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_subscribe_update failed with error %d; %d\n",
                subscribe_update_result.status, subscribe_update_result.underlying_error);
        return -1;
    }

    const ac_result_t subscribe_spot_result = ac_client_subscribe_spot(spotter);
    if (subscribe_spot_result.status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_subscribe_spot failed with error %d; %d\n",
                subscribe_spot_result.status, subscribe_spot_result.underlying_error);
        return -1;
    }

    for (int i = 0; i < 1000; i++) {
        receive_data(updater);
        receive_data(spotter);
    }

    const ac_result_t dismiss_updater_result = ac_client_dismiss(updater);
    if (dismiss_updater_result.status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_dismiss for updater failed with error %d; %d\n",
                dismiss_updater_result.status, dismiss_updater_result.underlying_error);
        return -1;
    }

    const ac_result_t dismiss_spotter_result = ac_client_dismiss(spotter);
    if (dismiss_spotter_result.status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_dismiss for spotter failed with error %d; %d\n",
                dismiss_spotter_result.status, dismiss_spotter_result.underlying_error);
        return -1;
    }

    const ac_result_t close_updater_result = ac_client_close(updater);
    if (close_updater_result.status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_close for updater failed with error %d; %d\n",
                close_updater_result.status, close_updater_result.underlying_error);
        return -1;
    }

    const ac_result_t close_spotter_result = ac_client_close(spotter);
    if (close_spotter_result.status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_close for spotter failed with error %d; %d\n",
                close_spotter_result.status, close_spotter_result.underlying_error);
        return -1;
    }

    return 0;
}
