#include "../include/libracing/ac.h"

#include <stdbool.h>
#include <stdlib.h>

#include "./utils/udp_socket.c"

#define AC_BUFFER_SIZE 408

static int32_t ac_last_error;
int32_t ac_get_last_error(void) { return ac_last_error; }

typedef struct AssettoCorsaClient {
  udp_socket_t *_udp_socket;
  bool _handshake_performed;
} ac_client_t;

ac_client_t *ac_client_create(const char *address, uint16_t port) {
  ac_client_t *client = malloc(sizeof(ac_client_t));
  if (client == NULL) {
    ac_last_error = AC_STATUS_ALLOCATION_FAILED;
    ac_client_destroy(client);
    return NULL;
  }

  client->_udp_socket = udp_socket_create(address, port);
  if (client->_udp_socket == NULL) {
    ac_last_error = udp_socket_get_last_error();
    ac_client_destroy(client);
    return NULL;
  }

  return client;
}

bool ac_client_destroy(ac_client_t *client) {
  if (client == NULL) {
    return false;
  }

  if (client->_handshake_performed == true) {
    bool has_error = ac_client_dismiss(client);
    if (has_error == true) {
      return true;
    }
  }

  if (client->_udp_socket != NULL) {
    bool has_error = udp_socket_destroy(client->_udp_socket);
    if (has_error == true) {
      ac_last_error = udp_socket_get_last_error();
      return true;
    }
  }

  free(client);
  return false;
}

void ac_fix_string(char *string, int32_t size) {
  bool found = false;

  for (int i = 0; i < size; i++) {
    if (string[i] == '%') {
      found = true;
    }
    if (found == true) {
      string[i] = '\0';
    }
  }
}

bool ac_client_send(ac_client_t *client, ac_operation_t operation) {
  if (client == NULL) {
    ac_last_error = AC_STATUS_CLIENT_NOT_INITIALIZED;
    return true;
  }
  if (client->_handshake_performed == false && operation != AC_OPERATION_HANDSHAKE) {
    ac_last_error = AC_STATUS_HANDSHAKE_NOT_PERFORMED;
    return true;
  }

  const ac_request_t request = {
      .identifier = AC_IDENTIFIER,
      .version = AC_SERVER_VERSION,
      .operation = operation,
  };

  int32_t bytes_send = udp_socket_send(client->_udp_socket, (char *)&request, sizeof(ac_request_t));
  if (bytes_send < 0) {
    ac_last_error = udp_socket_get_last_error();
    return true;
  }

  if (operation == AC_OPERATION_HANDSHAKE) {
    client->_handshake_performed = true;
  }
  if (operation == AC_OPERATION_DISMISS) {
    client->_handshake_performed = false;
  }

  return false;
}

bool ac_set_response_handshake(ac_event_t *event, char *buffer) {
  event->type = AC_EVENT_TYPE_HANDSHAKE;

  event->data.handshake = malloc(sizeof(ac_response_t));
  if (event->data.handshake == NULL) {
    ac_last_error = AC_STATUS_ALLOCATION_FAILED;
    return true;
  }

  memcpy(event->data.handshake, buffer, sizeof(ac_response_t));

  ac_fix_string(event->data.handshake->car_name, 100);
  ac_fix_string(event->data.handshake->driver_name, 100);
  ac_fix_string(event->data.handshake->track_name, 100);
  ac_fix_string(event->data.handshake->track_config, 100);

  return false;
}

bool ac_set_response_rt_car_info(ac_event_t *event, char *buffer) {
  event->type = AC_EVENT_TYPE_CAR_INFO;

  event->data.car_info = malloc(sizeof(ac_rt_car_info));
  if (event->data.car_info == NULL) {
    ac_last_error = AC_STATUS_ALLOCATION_FAILED;
    return true;
  }

  memcpy(event->data.car_info, buffer, sizeof(ac_rt_car_info));

  return false;
}

bool ac_set_response_rt_lap_info(ac_event_t *event, char *buffer) {
  event->type = AC_EVENT_TYPE_LAP_INFO;

  event->data.lap_info = malloc(sizeof(ac_rt_lap_info));
  if (event->data.lap_info == NULL) {
    ac_last_error = AC_STATUS_ALLOCATION_FAILED;
    return true;
  }

  memcpy(event->data.lap_info, buffer, sizeof(ac_rt_lap_info));

  ac_fix_string(event->data.lap_info->car_name, 100);
  ac_fix_string(event->data.lap_info->driver_name, 100);

  return false;
}

bool ac_client_receive(ac_client_t *client, ac_event_t *event) {
  if (client == NULL) {
    ac_last_error = AC_STATUS_CLIENT_NOT_INITIALIZED;
    return true;
  }
  if (client->_handshake_performed == false) {
    ac_last_error = AC_STATUS_HANDSHAKE_NOT_PERFORMED;
    return true;
  }

  char buffer[AC_BUFFER_SIZE];

  int32_t bytes_received = udp_socket_receive(client->_udp_socket, buffer, AC_BUFFER_SIZE);
  if (bytes_received < 0) {
    ac_last_error = udp_socket_get_last_error();
    return true;
  }

  switch (bytes_received) {
  case sizeof(ac_response_t):
    return ac_set_response_handshake(event, buffer);
  case sizeof(ac_rt_car_info):
    return ac_set_response_rt_car_info(event, buffer);
  case sizeof(ac_rt_lap_info):
    return ac_set_response_rt_lap_info(event, buffer);
  default:
    ac_last_error = AC_STATUS_RECEIVED_INVALID_DATA;
    return true;
  }
}

bool ac_client_handshake(ac_client_t *client) {
  return ac_client_send(client, AC_OPERATION_HANDSHAKE);
}

bool ac_client_subscribe_update(ac_client_t *client) {
  return ac_client_send(client, AC_OPERATION_SUBSCRIBE_UPDATE);
}

bool ac_client_subscribe_spot(ac_client_t *client) {
  return ac_client_send(client, AC_OPERATION_SUBSCRIBE_SPOT);
}

bool ac_client_dismiss(ac_client_t *client) {
  return ac_client_send(client, AC_OPERATION_DISMISS);
}
