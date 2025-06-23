#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "../include/libracing/acc.h"

#pragma comment(lib, "Ws2_32.lib")

static WSADATA acc_wsa_data;
static int acc_wsa_users = 0;
static int acc_address_size = sizeof(SOCKADDR_IN);

acc_status_t acc_client_send(acc_client_t *client, const char *data, const int size) {
    if (!client->_socket_initialized) return ACC_STATUS_NOT_INITIALIZED;
    if (!client->_address_initialized) return ACC_STATUS_NOT_INITIALIZED;

    const int sent = sendto(client->_socket, data, size, 0, (struct sockaddr *)&client->_address, acc_address_size);
    if (sent == SOCKET_ERROR) {
        fprintf(stderr, "ACC: sendto failed. Error: %d\n", WSAGetLastError());
        return ACC_STATUS_SOCKET_ERROR;
    }

    if (sent != size) {
        fprintf(stderr, "ACC: sendto failed. Size: %d; Send: %d\n", size, sent);
        return ACC_STATUS_SOCKET_ERROR;
    }

    return ACC_STATUS_OK;
}

acc_status_t acc_client_receive(acc_client_t *client, char *data, const int size) {
    if (!client->_socket_initialized) return ACC_STATUS_NOT_INITIALIZED;
    if (!client->_address_initialized) return ACC_STATUS_NOT_INITIALIZED;

    const int received = recvfrom(client->_socket, data, size, 0, (struct sockaddr *)&client->_address, &acc_address_size);
    if (received == SOCKET_ERROR) {
        fprintf(stderr, "ACC: recvfrom failed. Error: %d\n", WSAGetLastError());
        return ACC_STATUS_SOCKET_ERROR;
    }

    return ACC_STATUS_OK;
}

SOCKET acc_create_socket() {
    SOCKET acc_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (acc_socket == INVALID_SOCKET) {
        fprintf(stderr, "ACC: socket failed. Error: %d\n", WSAGetLastError());
        return INVALID_SOCKET;
    }

    return acc_socket;
}

acc_status_t acc_init_address(struct sockaddr_in *address, const int port, const char *ip) {
    ZeroMemory(address, acc_address_size);
    address->sin_family = AF_INET;
    address->sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &address->sin_addr) != 1) {
        fprintf(stderr, "ACC: inet_pton failed. Error: %d\n", WSAGetLastError());
        return ACC_STATUS_SOCKET_ERROR;
    }

    return ACC_STATUS_OK;
}

acc_status_t acc_bind_socket(SOCKET acc_socket, struct sockaddr_in *address) {
    if (bind(acc_socket, (struct sockaddr *)address, acc_address_size) == SOCKET_ERROR) {
        fprintf(stderr, "ACC: bind failed. Error: %d\n", WSAGetLastError());
        return ACC_STATUS_SOCKET_ERROR;
    }

    return ACC_STATUS_OK;
}

acc_client_t acc_new_client() {
    acc_client_t client = {
        .connection_id = 1000,
        ._socket_initialized = false,
        ._address_initialized = false,
        ._handshake_completed = false,
        ._socket = INVALID_SOCKET,
        ._address = { 0 },
    };

    return client;
}

acc_status_t acc_client_connect(acc_client_t *client, const char *server_ip, const int server_port) {
    if (client->_socket_initialized) return ACC_STATUS_ALREADY_INITIALIZED;
    if (client->_address_initialized) return ACC_STATUS_ALREADY_INITIALIZED;
    if (client->_handshake_completed) return ACC_STATUS_ALREADY_INITIALIZED;

    if (acc_wsa_users == 0) {
        const int wsa_startup_status = WSAStartup(MAKEWORD(2, 2), &acc_wsa_data);
        if (wsa_startup_status != 0) {
            fprintf(stderr, "ACC: WSAStartup failed. Error: %d; %d\n", WSAGetLastError(), wsa_startup_status);
            return ACC_STATUS_SOCKET_ERROR;
        }
    }

    acc_wsa_users++;

    client->_socket = acc_create_socket();
    if (client->_socket == INVALID_SOCKET) return ACC_STATUS_SOCKET_ERROR;
    client->_socket_initialized = true;

    const acc_status_t address_status = acc_init_address(&client->_address, server_port, server_ip);
    if (address_status != ACC_STATUS_OK) return address_status;
    client->_address_initialized = true;

    return ACC_STATUS_OK;
}

acc_status_t acc_client_disconnect(acc_client_t *client) {
	if (client->_handshake_completed) return ACC_STATUS_NOT_DISMISSED;

    if (client->_address_initialized) {
        ZeroMemory(&client->_address, acc_address_size);
        client->_address_initialized = false;
    }

    if (client->_socket_initialized) {
        closesocket(client->_socket);
        client->_socket = INVALID_SOCKET;
        client->_socket_initialized = false;
    }

    if (acc_wsa_users > 0) {
        acc_wsa_users--;
        if (acc_wsa_users == 0) {
            WSACleanup();
            ZeroMemory(&acc_wsa_data, sizeof(acc_wsa_data));
        }
    }

    return ACC_STATUS_OK;
}

int acc_write_byte(char *buffer, int offset, const acc_outbound_message_type_t type) {
    buffer[offset++] = (char)type;
    return offset;
}

int acc_write_short(char *buffer, int offset, const short data) {
    char bytes[2];
    memcpy(bytes, &data, 2);

    for (int i = 0; i < 2; i++) {
        buffer[offset++] = bytes[i];
    }

    return offset;
}

int acc_write_int(char *buffer, int offset, const int data) {
    char bytes[4];
    memcpy(bytes, &data, 4);

    for (int i = 0; i < 4; i++) {
        buffer[offset++] = bytes[i];
    }

    return offset;
}

int acc_write_float(char *buffer, int offset, const float data) {
    char bytes[4];
    memcpy(bytes, &data, 4);

    for (int i = 0; i < 4; i++) {
        buffer[offset++] = bytes[i];
    }

    return offset;
}

int acc_write_string(char *buffer, int offset, const char *data, const int data_size) {
    for (int i = 0; i < data_size; i++) {
        buffer[offset++] = data[i];
        buffer[offset++] = '0';
    }

    return offset;
}

acc_status_t acc_client_send(acc_client_t *client, const acc_outbound_message_type_t type, acc_server_request_t request) {
    int total = 1; // 1. Request Type
    int offset = 0;
    char *buffer = NULL;

    switch (type) {
        case ACC_OUTBOUND_MESSAGE_REGISTER_COMMAND_APPLICATION:
            total += 1; // 2. Broadcasting Version
            total += request.register_application->display_name_size * 2; // 3. Display Name
            total += request.register_application->connection_password_size * 2; // 4. Connection Password
            total += 4; // 5. Update Interval
            total += request.register_application->command_password_size * 2; // 6. Command Password

            buffer = (char *)malloc(total);
            offset = acc_write_byte(buffer, offset, type);
            offset = acc_write_byte(buffer, offset, ACC_BROADCASTING_PROTOCOL_VERSION);
            offset = acc_write_string(buffer, offset, request.register_application->display_name, request.register_application->display_name_size);
            offset = acc_write_string(buffer, offset, request.register_application->connection_password, request.register_application->connection_password_size);
            offset = acc_write_int(buffer, offset, request.register_application->update_interval);
            offset = acc_write_string(buffer, offset, request.register_application->command_password, request.register_application->command_password_size);

            break;
        case ACC_OUTBOUND_MESSAGE_UNREGISTER_COMMAND_APPLICATION:
            buffer = (char *)malloc(total);
            offset = acc_write_byte(buffer, offset, type);
            break;
        case ACC_OUTBOUND_MESSAGE_REQUEST_ENTRY_LIST:
            total += 4; // 2. Connection ID
            buffer = (char *)malloc(total);
            offset = acc_write_byte(buffer, offset, type);
            offset = acc_write_int(buffer, offset, request.request_entry_list->connection_id);
            break;
        case ACC_OUTBOUND_MESSAGE_REQUEST_TRACK_DATA:
            total += 4; // 2. Connection ID
            buffer = (char *)malloc(total);
            offset = acc_write_byte(buffer, offset, type);
            offset = acc_write_int(buffer, offset, request.request_track_data->connection_id);
            break;
        case ACC_OUTBOUND_MESSAGE_CHANGE_HUD_PAGE:
            total += 4; // 2. Connection ID
            total += request.request_hud_page->hud_page_size * 2; // 3. HUD Page

            buffer = (char *)malloc(total);
            offset = acc_write_byte(buffer, offset, type);
            offset = acc_write_int(buffer, offset, request.request_hud_page->connection_id);
            offset = acc_write_string(buffer, offset, request.request_hud_page->hud_page, request.request_hud_page->hud_page_size);
            break;
        case ACC_OUTBOUND_MESSAGE_CHANGE_FOCUS:
            total += 4; // 2. Connection ID
            total += 1; // 3.1. Car Index Indicator
            if (request.change_focus->car_index != NULL) total += 2; // 3.2. Car Index
            total += 1; // 4.1. Camera Indicator
            if (request.change_focus->camera_set_size != 0) total += request.change_focus->camera_set_size * 2; // 4.2. Camera Set
            if (request.change_focus->camera_size != 0) total += request.change_focus->camera_size * 2; // 4.2. Camera

            buffer = (char *)malloc(total);
            offset = acc_write_byte(buffer, offset, type);
            offset = acc_write_int(buffer, offset, request.change_focus->connection_id);
            if (request.change_focus->car_index != NULL) {
                offset = acc_write_byte(buffer, offset, 1);
                offset = acc_write_short(buffer, offset, *request.change_focus->car_index);
            } else {
                offset = acc_write_byte(buffer, offset, 0);
            }
            if (request.change_focus->camera_set_size != 0 || request.change_focus->camera_size != 0) {
                offset = acc_write_byte(buffer, offset, 1);
                if (request.change_focus->camera_set_size != 0) {
                    offset = acc_write_string(buffer, offset, request.change_focus->camera_set, request.change_focus->camera_set_size);
                }
                if (request.change_focus->camera_size != 0) {
                    offset = acc_write_string(buffer, offset, request.change_focus->camera, request.change_focus->camera_size);
                }
            } else {
                offset = acc_write_byte(buffer, offset, 0);
            }
            break;
        case ACC_OUTBOUND_MESSAGE_INSTANT_REPLAY_REQUEST:
            total += 4; // 2. Connection ID
            total += 4; // 3. Start Session Time
            total += 4; // 4. Duration
            total += 4; // 5. Car Index Indicator
            total += request.request_instant_replay->initial_camera_set_size * 2; // 6. Initial Camera Set
            total += request.request_instant_replay->initial_camera_size * 2; // 7. Initial Camera

            buffer = (char *)malloc(total);
            offset = acc_write_byte(buffer, offset, type);
            offset = acc_write_int(buffer, offset, request.request_instant_replay->connection_id);
            offset = acc_write_float(buffer, offset, request.request_instant_replay->start_session_time);
            offset = acc_write_float(buffer, offset, request.request_instant_replay->duration);
            offset = acc_write_int(buffer, offset, request.request_instant_replay->initial_focused_car_index);
            offset = acc_write_string(buffer, offset, request.request_instant_replay->initial_camera_set, request.request_instant_replay->initial_camera_set_size);
            offset = acc_write_string(buffer, offset, request.request_instant_replay->initial_camera, request.request_instant_replay->initial_camera_size);
            break;
        default: ;
    }

    if (total != offset) {
        fprintf(stderr, "ACC: Buffer size mismatch. Expected: %d; Actual: %d\n", total, offset);
    }

    if (buffer != NULL) {
        const acc_status_t send_status = acc_client_send(client, buffer, total);
        free(buffer);
        return send_status;
    }

    return ACC_STATUS_OK;
}

acc_status_t acc_client_receive(acc_client_t *client, acc_inbound_message_type_t *type, acc_server_response_t *response) {
    return ACC_STATUS_OK;
}