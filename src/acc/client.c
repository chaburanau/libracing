#include "./types.c"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../utils/udp_socket.c"
#include "./reader.c"
#include "./writer.c"

#define ACC_RECEIVE_BUFFER_SIZE 1024 * 100

static int32_t acc_last_error = 0;
int32_t acc_get_last_error(void) { return acc_last_error; }

typedef struct ACClient {
    udp_socket_t *socket;
} acc_client_t;

acc_client_t *acc_client_create(char *address, uint16_t port);
bool acc_client_destroy(acc_client_t *client);
bool acc_client_send(acc_client_t *client, acc_server_request_t *request);
bool acc_client_receive(acc_client_t *client, acc_server_response_t *response);

acc_client_t *acc_client_create(char *address, uint16_t port) {
    acc_client_t *client = malloc(sizeof(acc_client_t));
    if (client == NULL) {
        acc_last_error = ACC_STATUS_MEMORY_ALLOCATION_ERROR;
        acc_client_destroy(client);
        return NULL;
    }

    client->socket = udp_socket_create(address, port);
    if (client->socket == NULL) {
        acc_last_error = udp_socket_get_last_error();
        acc_client_destroy(client);
        return NULL;
    }

    return client;
}

bool acc_client_destroy(acc_client_t *client) {
    if (client == NULL) {
        return false;
    }

    if (client->socket != NULL) {
        bool has_error = udp_socket_destroy(client->socket);
        if (has_error == true) {
            acc_last_error = udp_socket_get_last_error();
            return true;
        }
    }

    free(client);
    return false;
}

bool acc_client_send(acc_client_t *client, acc_server_request_t *request) {
    if (client == NULL) {
        acc_last_error = ACC_STATUS_CLIENT_NOT_INITIALIZED;
        return true;
    }

    if (client->socket == NULL) {
        acc_last_error = ACC_STATUS_CLIENT_NOT_INITIALIZED;
        return true;
    }

    char *buffer = NULL;
    size_t total = 0;
    int32_t status = 0;

    switch (request->type) {
    case ACC_OUTBOUND_MESSAGE_REGISTER_COMMAND_APPLICATION:
        status = acc_write_register_application(&buffer, &total, request->data.register_application);
        break;
    case ACC_OUTBOUND_MESSAGE_UNREGISTER_COMMAND_APPLICATION:
        status = acc_write_unregister_application(&buffer, &total, request->data.unregister_application);
        break;
    case ACC_OUTBOUND_MESSAGE_REQUEST_ENTRY_LIST:
        status = acc_write_request_entry_list(&buffer, &total, request->data.request_entry_list);
        break;
    case ACC_OUTBOUND_MESSAGE_REQUEST_TRACK_DATA:
        status = acc_write_request_track_data(&buffer, &total, request->data.request_track_data);
        break;
    case ACC_OUTBOUND_MESSAGE_CHANGE_HUD_PAGE:
        status = acc_write_change_hud_page(&buffer, &total, request->data.change_hud_page);
        break;
    case ACC_OUTBOUND_MESSAGE_CHANGE_FOCUS:
        status = acc_write_change_focus(&buffer, &total, request->data.change_focus);
        break;
    case ACC_OUTBOUND_MESSAGE_INSTANT_REPLAY_REQUEST:
        status = acc_write_instant_replay_request(&buffer, &total, request->data.request_instant_replay);
        break;
    default:
        acc_last_error = ACC_STATUS_INVALID_OUTBOUND_MESSAGE_TYPE;
        return true;
    }

    if (status != ACC_STATUS_OK) {
        acc_last_error = status;
        free(buffer);
        return true;
    }

    int32_t sent = udp_socket_send(client->socket, buffer, total);
    if (sent < 0) {
        acc_last_error = udp_socket_get_last_error();
        free(buffer);
        return true;
    }

    if ((size_t)sent != total) {
        acc_last_error = ACC_STATUS_UNEXPECTED_AMOUNT_OF_BYTES_SENT;
        free(buffer);
        return true;
    }

    free(buffer);
    return false;
}

bool acc_client_receive(acc_client_t *client, acc_server_response_t *response) {
    if (client == NULL) {
        acc_last_error = ACC_STATUS_CLIENT_NOT_INITIALIZED;
        return true;
    }
    if (client->socket == NULL) {
        acc_last_error = ACC_STATUS_CLIENT_NOT_INITIALIZED;
        return true;
    }

    char buffer[ACC_RECEIVE_BUFFER_SIZE];

    int32_t received = udp_socket_receive(client->socket, buffer, ACC_RECEIVE_BUFFER_SIZE);
    if (received < 0) {
        acc_last_error = udp_socket_get_last_error();
        return true;
    }

    size_t offset = 1;
    response->type = (acc_inbound_message_type_t)buffer[0];

    switch (response->type) {
    case ACC_INBOUND_MESSAGE_REGISTRATION_RESULT:
        response->data.registration_result = malloc(sizeof(acc_reg_result_t));
        if (response->data.registration_result == NULL) {
            acc_last_error = ACC_STATUS_MEMORY_ALLOCATION_ERROR;
            return true;
        }

        acc_read_registration_result(response->data.registration_result, buffer, &offset);
        break;
    case ACC_INBOUND_MESSAGE_REAL_TIME_UPDATE:
        response->data.real_time_update = malloc(sizeof(acc_rt_update_t));
        if (response->data.real_time_update == NULL) {
            acc_last_error = ACC_STATUS_MEMORY_ALLOCATION_ERROR;
            return true;
        }

        acc_read_real_time_update(response->data.real_time_update, buffer, &offset);
        break;
    case ACC_INBOUND_MESSAGE_REAL_TIME_CAR_UPDATE:
        response->data.real_time_car_update = malloc(sizeof(acc_rt_car_update_t));
        if (response->data.real_time_car_update == NULL) {
            acc_last_error = ACC_STATUS_MEMORY_ALLOCATION_ERROR;
            return true;
        }

        acc_read_real_time_car_update(response->data.real_time_car_update, buffer, &offset);
        break;
    case ACC_INBOUND_MESSAGE_ENTRY_LIST:
        response->data.entry_list = malloc(sizeof(acc_entry_list_t));
        if (response->data.entry_list == NULL) {
            acc_last_error = ACC_STATUS_MEMORY_ALLOCATION_ERROR;
            return true;
        }

        acc_read_entry_list(response->data.entry_list, buffer, &offset);
        break;
    case ACC_INBOUND_MESSAGE_ENTRY_LIST_CAR:
        response->data.entry_list_car = malloc(sizeof(acc_car_info_t));
        if (response->data.entry_list_car == NULL) {
            acc_last_error = ACC_STATUS_MEMORY_ALLOCATION_ERROR;
            return true;
        }

        acc_read_entry_list_car(response->data.entry_list_car, buffer, &offset);
        break;
    case ACC_INBOUND_MESSAGE_TRACK_DATA:
        response->data.track_data = malloc(sizeof(acc_track_data_t));
        if (response->data.track_data == NULL) {
            acc_last_error = ACC_STATUS_MEMORY_ALLOCATION_ERROR;
            return true;
        }

        acc_read_track_data(response->data.track_data, buffer, &offset);
        break;
    case ACC_INBOUND_MESSAGE_BROADCASTING_EVENT:
        response->data.broadcasting_event = malloc(sizeof(acc_broadcasting_event_t));
        if (response->data.broadcasting_event == NULL) {
            acc_last_error = ACC_STATUS_MEMORY_ALLOCATION_ERROR;
            return true;
        }

        acc_read_broadcasting_event(response->data.broadcasting_event, buffer, &offset);
        break;
    default:
        acc_last_error = ACC_STATUS_INVALID_INBOUND_MESSAGE_TYPE;
        return true;
    }

    if (offset > ACC_RECEIVE_BUFFER_SIZE) {
        acc_last_error = ACC_STATUS_INBOUND_BUFFER_TOO_SMALL;
        return true;
    }

    if (offset > (size_t)received) {
        acc_last_error = ACC_STATUS_INBOUND_BUFFER_TOO_SMALL;
        return true;
    }

    return false;
}
