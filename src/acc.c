#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "../include/libracing/acc.h"

#pragma comment(lib, "Ws2_32.lib")

static WSADATA acc_wsa_data;
static int acc_wsa_users = 0;
static int acc_address_size = sizeof(struct sockaddr_in);

acc_status_t acc_client_send(acc_client_t *client) {
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
        ._server_socket_initialized = false,
        ._client_socket_initialized = false,
        ._server_address_initialized = false,
        ._client_address_initialized = false,
        ._client_socket_binded = false,
        ._handshake_initialized = false,
        ._server_socket = INVALID_SOCKET,
        ._client_socket = INVALID_SOCKET,
        ._server_address = { 0 },
        ._client_address = { 0 },
    };

    return client;
}

acc_status_t acc_client_connect(acc_client_t *client, const char *server_ip, const char *client_ip, const int server_port, const int client_port) {
    if (client->_server_socket_initialized) return ACC_STATUS_ALREADY_INITIALIZED;
    if (client->_client_socket_initialized) return ACC_STATUS_ALREADY_INITIALIZED;
    if (client->_server_address_initialized) return ACC_STATUS_ALREADY_INITIALIZED;
    if (client->_client_address_initialized) return ACC_STATUS_ALREADY_INITIALIZED;
    if (client->_client_socket_binded) return ACC_STATUS_ALREADY_INITIALIZED;
    if (client->_handshake_initialized) return ACC_STATUS_ALREADY_INITIALIZED;

    if (acc_wsa_users == 0) {
        const int wsa_startup_status = WSAStartup(MAKEWORD(2, 2), &acc_wsa_data);
        if (wsa_startup_status != 0) {
            fprintf(stderr, "ACC: WSAStartup failed. Error: %d; %d\n", WSAGetLastError(), wsa_startup_status);
            return ACC_STATUS_SOCKET_ERROR;
        }
    }

    acc_wsa_users++;

    client->_server_socket = acc_create_socket();
    if (client->_server_socket == INVALID_SOCKET) return ACC_STATUS_SOCKET_ERROR;
    client->_server_socket_initialized = true;

    client->_client_socket = acc_create_socket();
    if (client->_client_socket == INVALID_SOCKET) return ACC_STATUS_SOCKET_ERROR;
    client->_client_socket_initialized = true;

    const acc_status_t server_address_status = acc_init_address(&client->_server_address, server_port, server_ip);
    if (server_address_status != ACC_STATUS_OK) return server_address_status;
    client->_server_address_initialized = true;

    const acc_status_t client_address_status = acc_init_address(&client->_client_address, client_port, client_ip);
    if (client_address_status != ACC_STATUS_OK) return client_address_status;
    client->_client_address_initialized = true;

    const acc_status_t bind_client_socket_status = acc_bind_socket(client->_client_socket, &client->_client_address);
    if (bind_client_socket_status != ACC_STATUS_OK) return bind_client_socket_status;
    client->_client_socket_binded = true;

    return ACC_STATUS_OK;
}

acc_status_t acc_client_disconnect(acc_client_t *client) {
	if (client->_handshake_initialized) return ACC_STATUS_NOT_DISMISSED;

    if (client->_client_address_initialized) {
        ZeroMemory(&client->_client_address, acc_address_size);
        client->_client_address_initialized = false;
    }

    if (client->_server_address_initialized) {
        ZeroMemory(&client->_server_socket, acc_address_size);
        client->_server_address_initialized = false;
    }

    if (client->_client_socket_initialized) {
        closesocket(client->_client_socket);
        client->_client_socket = INVALID_SOCKET;
        client->_client_socket_initialized = false;
    }

    if (client->_server_address_initialized) {
        closesocket(client->_server_socket);
        client->_server_socket = INVALID_SOCKET;
        client->_server_socket_initialized = false;
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

acc_status_t acc_client_request_entry_list(acc_client_t *client) {
    return ACC_STATUS_OK;
}

acc_status_t acc_client_request_track_data(acc_client_t *client) {
    return ACC_STATUS_OK;
}

acc_status_t acc_client_request_change_focus(acc_client_t *client, int car_index, char *camera_set, char *camera) {
    return ACC_STATUS_OK;
}

acc_status_t acc_client_request_instant_replay_request(acc_client_t *client, float start_session_time, float duration, int car_index, char *camera_set, char *camera) {
    return ACC_STATUS_OK;
}

acc_status_t acc_client_request_hud_page(acc_client_t *client, char *hud_page) {
    return ACC_STATUS_OK;
}