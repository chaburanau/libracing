#include <stdio.h>

#include "../include/libracing/acc.h"

int acc_write_int8_t(char *buffer, int32_t offset, const int8_t type) {
    buffer[offset++] = type;
    return offset;
}

int acc_write_uint16_t(char *buffer, int32_t offset, const uint16_t data) {
    const size_t size = sizeof(uint16_t);
    char bytes[size];
    memcpy(bytes, &data, size);

    for (int i = 0; i < size; i++) {
        buffer[offset++] = bytes[i];
    }

    return offset;
}

int acc_write_int32_t(char *buffer, int32_t offset, const int32_t data) {
    const size_t size = sizeof(int32_t);
    char bytes[size];
    memcpy(bytes, &data, size);

    for (int i = 0; i < size; i++) {
        buffer[offset++] = bytes[i];
    }

    return offset;
}

int acc_write_float(char *buffer, int offset, const float data) {
    const size_t size = sizeof(int32_t);
    char bytes[size];
    memcpy(bytes, &data, size);

    for (int i = 0; i < size; i++) {
        buffer[offset++] = bytes[i];
    }

    return offset;
}

int acc_write_string(char *buffer, int offset, const string_t *string) {
    for (int i = 0; i < string->length; i++) {
        buffer[offset++] = string->data[i];
        buffer[offset++] = '\0';
    }

    return offset;
}

int32_t acc_write_register_application(char **buffer, const acc_reg_app_t *reg_app) {
    int32_t total = 0;
    int32_t offset = 0;

    total += sizeof(int8_t);                            // 1. Request Type
    total += sizeof(int8_t);                            // 2. Broadcasting Version
    total += reg_app->display_name->length * 2;         // 3. Display Name
    total += reg_app->connection_password->length * 2;  // 4. Connection Password
    total += sizeof(int32_t);                           // 5. Update Interval
    total += reg_app->command_password->length * 2;     // 6. Command Password

    *buffer = malloc(total);

    offset = acc_write_int8_t(*buffer, offset, ACC_OUTBOUND_MESSAGE_REGISTER_COMMAND_APPLICATION);
    offset = acc_write_int8_t(*buffer, offset, ACC_BROADCASTING_PROTOCOL_VERSION);
    offset = acc_write_string(*buffer, offset, reg_app->display_name);
    offset = acc_write_string(*buffer, offset, reg_app->connection_password);
    offset = acc_write_int32_t(*buffer, offset, reg_app->update_interval);
    offset = acc_write_string(*buffer, offset, reg_app->command_password);

    return offset != total ? total : -1;
}

int32_t acc_write_unregister_application(char **buffer, const acc_unreg_app_t *unreg_app) {
    int32_t total = sizeof(int8_t); // 1. Request Type
    int32_t offset = 0;

    *buffer = malloc(total);

    offset = acc_write_int8_t(*buffer, offset, ACC_OUTBOUND_MESSAGE_UNREGISTER_COMMAND_APPLICATION);

    return offset != total ? total : -1;
}

int32_t acc_write_request_entry_list(char **buffer, const acc_req_entry_list_t *req_entry_list) {
    int32_t total = 0;
    int32_t offset = 0;

    total += sizeof(int8_t);    // 1. Request Type
    total += sizeof(int32_t);   // 2. Connection ID

    *buffer = malloc(total);

    offset = acc_write_int8_t(*buffer, offset, ACC_OUTBOUND_MESSAGE_REQUEST_ENTRY_LIST);
    offset = acc_write_int32_t(*buffer, offset, req_entry_list->connection_id);

    return offset != total ? total : -1;
}

int32_t acc_write_request_track_data(char **buffer, const acc_req_track_data_t *req_track_data) {
    int32_t total = 0;
    int32_t offset = 0;

    total += sizeof(int8_t);    // 1. Request Type
    total += sizeof(int32_t);   // 2. Connection ID

    *buffer = malloc(total);

    offset = acc_write_int8_t(*buffer, offset, ACC_OUTBOUND_MESSAGE_REQUEST_TRACK_DATA);
    offset = acc_write_int32_t(*buffer, offset, req_track_data->connection_id);

    return offset != total ? total : -1;
}

int32_t acc_write_change_hud_page(char **buffer, const acc_change_hud_page_t *change_hud_page) {
    int32_t total = 0;
    int32_t offset = 0;

    total += sizeof(int8_t);    // 1. Request Type
    total += sizeof(int32_t);   // 2. Connection ID
    total += change_hud_page->hud_page->length * 2; // 3. HUD Page

    *buffer = malloc(total);

    offset = acc_write_int8_t(*buffer, offset, ACC_OUTBOUND_MESSAGE_CHANGE_HUD_PAGE);
    offset = acc_write_int32_t(*buffer, offset, change_hud_page->connection_id);
    offset = acc_write_string(*buffer, offset, change_hud_page->hud_page);

    return offset != total ? total : -1;
}

int32_t acc_write_change_focus(char **buffer, const acc_change_focus_t *change_focus) {
    int32_t total = 0;
    int32_t offset = 0;

    total += sizeof(int8_t);                                                                // 1. Request Type
    total += sizeof(int32_t);                                                               // 2. Connection ID
    total += sizeof(int8_t);                                                                // 3.1. Car Index Indicator
    if (change_focus->car_index != NULL) total += sizeof(uint16_t);                         // 3.2. Car Index
    total += sizeof(int8_t);                                                                // 4.1. Camera Indicator
    if (change_focus->camera_set != NULL) total += change_focus->camera_set->length * 2;    // 4.2. Camera Set
    if (change_focus->camera != NULL) total += change_focus->camera->length * 2;            // 4.2. Camera

    *buffer = malloc(total);

    offset = acc_write_int8_t(*buffer, offset, ACC_OUTBOUND_MESSAGE_CHANGE_FOCUS);
    offset = acc_write_int32_t(*buffer, offset, change_focus->connection_id);
    if (change_focus->car_index != NULL) {
        offset = acc_write_int8_t(*buffer, offset, 1);
        offset = acc_write_uint16_t(*buffer, offset, *change_focus->car_index);
    } else {
        offset = acc_write_int8_t(*buffer, offset, 0);
    }
    if (change_focus->camera_set != NULL || change_focus->camera != NULL) {
        offset = acc_write_int8_t(*buffer, offset, 1);
        if (change_focus->camera_set != 0) {
            offset = acc_write_string(*buffer, offset, change_focus->camera_set);
        }
        if (change_focus->camera != 0) {
            offset = acc_write_string(*buffer, offset, change_focus->camera);
        }
    }

    return offset != total ? total : -1;
}

int32_t acc_write_instant_replay_request(char **buffer, acc_req_instant_replay_t *req_instant_replay) {
    int32_t total = 0;
    int32_t offset = 0;

    total += sizeof(int8_t);                                        // 1. Request Type
    total += sizeof(int32_t);                                       // 2. Connection ID
    total += sizeof(float);                                         // 3. Start Session Time
    total += sizeof(float);                                         // 4. Duration
    total += sizeof(int32_t);                                       // 5. Car Index Indicator
    total += req_instant_replay->initial_camera_set->length * 2;    // 6. Initial Camera Set
    total += req_instant_replay->initial_camera->length * 2;        // 7. Initial Camera

    *buffer = malloc(total);

    offset = acc_write_int8_t(*buffer, offset, ACC_OUTBOUND_MESSAGE_INSTANT_REPLAY_REQUEST);
    offset = acc_write_int32_t(*buffer, offset, req_instant_replay->connection_id);
    offset = acc_write_float(*buffer, offset, req_instant_replay->start_session_time);
    offset = acc_write_float(*buffer, offset, req_instant_replay->duration);
    offset = acc_write_int32_t(*buffer, offset, req_instant_replay->initial_focused_car_index);
    offset = acc_write_string(*buffer, offset, req_instant_replay->initial_camera_set);
    offset = acc_write_string(*buffer, offset, req_instant_replay->initial_camera);

    return offset != total ? total : -1;
}

acc_client_t *acc_client_create(const char *address, const int port) {
    acc_client_t *client = malloc(sizeof(acc_client_t));
    if (client == NULL) {
        acc_client_close(client);
        return NULL;
    }

    client->socket = udp_socket_create(address, port);
    if (client->socket == NULL) {
        acc_client_close(client);
        return NULL;
    }

    return client;
}

void acc_client_close(acc_client_t *client) {
    if (client == NULL) return;
    if (client->socket != NULL) udp_socket_close(client->socket);
    free(client);
}

acc_status_t acc_client_send(acc_client_t *client, const acc_outbound_message_type_t type, acc_server_request_t request) {
    if (client == NULL) return ACC_STATUS_CLIENT_NOT_INITIALIZED;
    if (client->socket == NULL) return ACC_STATUS_CLIENT_NOT_INITIALIZED;

    char *buffer = NULL;
    int32_t total = 0;

    switch (type) {
        case ACC_OUTBOUND_MESSAGE_REGISTER_COMMAND_APPLICATION:
            total = acc_write_register_application(&buffer, request.register_application);
            break;
        case ACC_OUTBOUND_MESSAGE_UNREGISTER_COMMAND_APPLICATION:
            total = acc_write_unregister_application(&buffer, request.unregister_application);
            break;
        case ACC_OUTBOUND_MESSAGE_REQUEST_ENTRY_LIST:
            total = acc_write_request_entry_list(&buffer, request.request_entry_list);
            break;
        case ACC_OUTBOUND_MESSAGE_REQUEST_TRACK_DATA:
            total = acc_write_request_track_data(&buffer, request.request_track_data);
            break;
        case ACC_OUTBOUND_MESSAGE_CHANGE_HUD_PAGE:
            total = acc_write_change_hud_page(&buffer, request.request_hud_page);
            break;
        case ACC_OUTBOUND_MESSAGE_CHANGE_FOCUS:
            total = acc_write_change_focus(&buffer, request.change_focus);
            break;
        case ACC_OUTBOUND_MESSAGE_INSTANT_REPLAY_REQUEST:
            total = acc_write_instant_replay_request(&buffer, request.request_instant_replay);
            break;
        default:
            fprintf(stderr, "ACC: Invalid Outbound Message Type: %d\n", type);
            return ACC_STATUS_INVALID_OUTBOUND_MESSAGE_TYPE;
    }

    if (total <= 0) {
        fprintf(stderr, "ACC: acc_write_outbound_message_type failed\n");
        free(buffer);
        return ACC_STATUS_BUFFER_SIZE_MISMATCH;
    }

    const int sent = udp_socket_send(client->socket, buffer, total);
    if (sent < 0) {
        fprintf(stderr, "ACC: udp_socket_send failed\n");
        free(buffer);
        return ACC_STATUS_SOCKET_ERROR;
    }

    if (sent != total) {
        fprintf(stderr, "ACC: Unexpected amount of bytes send: Expected: %d; Actual: %d\n", total, sent);
        free(buffer);
        return ACC_STATUS_UNEXPECTED_AMOUNT_OF_BYTES_SENT;
    }

    free(buffer);
    return sent == total ? ACC_STATUS_OK : ACC_STATUS_SEND_ERROR;
}

acc_status_t acc_client_receive(acc_client_t *client, acc_inbound_message_type_t *type, acc_server_response_t *response) {
    if (client == NULL) return ACC_STATUS_CLIENT_NOT_INITIALIZED;
    if (client->socket == NULL) return ACC_STATUS_CLIENT_NOT_INITIALIZED;

    const int size = 1024 * 1024 * 10;
    char *buffer = malloc(size);

    const int received = udp_socket_receive(client->socket, buffer, size);
    if (received < 0) {
        free(buffer);
        fprintf(stderr, "ACC: udp_socket_receive failed\n");
        return ACC_STATUS_SOCKET_ERROR;
    }

    free(buffer);
    return ACC_STATUS_OK;
}