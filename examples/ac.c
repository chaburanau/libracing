#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define __STD_UTF_16__

#include "../src/ac.c"

void check_error(bool has_error, char *operation) {
  if (has_error == true) {
    fprintf(stderr, "%s(...) failed with error %d\n", operation, ac_get_last_error());
    exit(-1);
  }
}

void receive_data(ac_client_t *client) {
  ac_event_t event;

  bool has_error = ac_client_receive(client, &event);
  check_error(has_error, "ac_client_receive");

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
  (void)argc;
  (void)argv;

  fprintf(stdout, "Starting...\n");

  ac_client_t *updater = ac_client_create("127.0.0.1", 9996);
  if (updater == NULL)
    check_error(true, "ac_client_create");
  ac_client_t *spotter = ac_client_create("127.0.0.1", 9996);
  if (spotter == NULL)
    check_error(true, "ac_client_create");

  fprintf(stdout, "Clients have been initialized...\n");

  bool has_error;

  has_error = ac_client_handshake(updater);
  check_error(has_error, "ac_client_handshake");
  has_error = ac_client_handshake(spotter);
  check_error(has_error, "ac_client_handshake");

  fprintf(stdout, "Handshakes have been initialized...\n");

  receive_data(updater);
  receive_data(spotter);

  fprintf(stdout, "Data have been received...\n");

  has_error = ac_client_subscribe_update(updater);
  check_error(has_error, "ac_client_subscribe_update");
  has_error = ac_client_subscribe_spot(spotter);
  check_error(has_error, "ac_client_subscribe_spot");

  fprintf(stdout, "Clients have been subscribed...\n");

  for (int i = 0; i < 100; i++) {
    receive_data(updater);
    receive_data(spotter);
  }

  fprintf(stdout, "Data have been received...\n");

  has_error = ac_client_dismiss(updater);
  check_error(has_error, "ac_client_dismiss");
  has_error = ac_client_dismiss(spotter);
  check_error(has_error, "ac_client_dismiss");

  fprintf(stdout, "Clients have been dismissed...\n");

  has_error = ac_client_destroy(updater);
  check_error(has_error, "ac_client_destroy");
  has_error = ac_client_destroy(spotter);
  check_error(has_error, "ac_client_destroy");

  fprintf(stdout, "Clients have been destroyed...\n");
  fprintf(stdout, "Finishing...\n");

  return 0;
}
