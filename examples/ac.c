#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../src/ac.c"

void receive_data(ac_client_t *client) {
    ac_event_t event;

    const ac_status_t received_status = ac_client_receive(client, &event);
    if (received_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_receive() failed with error %d\n", received_status);
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
    ac_client_t* updater = ac_client_create("127.0.0.1", 9996);
    ac_client_t* spotter = ac_client_create("127.0.0.1", 9996);

    const ac_status_t handshake_updater_status = ac_client_handshake(updater);
    if (handshake_updater_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_handshake for updater failed with error %d\n", handshake_updater_status);
        return -1;
    }

    const ac_status_t handshake_spotter_status = ac_client_handshake(spotter);
    if (handshake_spotter_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_handshake for spotter failed with error %d\n", handshake_spotter_status);
        return -1;
    }

    receive_data(updater);
    receive_data(spotter);

    const ac_status_t subscribe_update_status = ac_client_subscribe_update(updater);
    if (subscribe_update_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_subscribe_update failed with error %d\n", subscribe_update_status);
        return -1;
    }

    const ac_status_t subscribe_spot_status = ac_client_subscribe_spot(spotter);
    if (subscribe_spot_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_subscribe_spot failed with error %d\n", subscribe_spot_status);
        return -1;
    }

    for (int i = 0; i < 100; i++) {
        receive_data(updater);
        receive_data(spotter);
    }

    const ac_status_t dismiss_updater_status = ac_client_dismiss(updater);
    if (dismiss_updater_status != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_dismiss for updater failed with error %d\n", dismiss_updater_status);
        return -1;
    }

    const ac_status_t dismiss_spotter_result = ac_client_dismiss(spotter);
    if (dismiss_spotter_result != AC_STATUS_OK) {
        fprintf(stderr, "ac_client_dismiss for spotter failed with error %d\n", dismiss_spotter_result);
        return -1;
    }

    ac_client_destroy(updater);
    ac_client_destroy(spotter);

    return 0;
}
