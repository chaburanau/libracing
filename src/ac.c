#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdint.h>

#include "../include/libracing/ac.h"

#pragma comment(lib, "Ws2_32.lib")

static int ac_server_address_size = sizeof(struct sockaddr_in);
static int ac_request_size = sizeof(ac_request_t);

static WSADATA ac_wsa_data;
static bool ac_wsa_initialized = false;
static int ac_wsa_usage_count = 0;

ac_client_t ac_new_client() {
	ac_client_t client = {
		._socket_initialized = false,
		._address_initialized = false,
		._handshake_initialized = false,
		._socket = INVALID_SOCKET,
		._server_address = { 0 },
	};

	return client;
}

ac_status_t ac_client_init(ac_client_t *client) {
	if (client->_socket_initialized) return AC_STATUS_ALREADY_INITIALIZED;
	if (client->_address_initialized) return AC_STATUS_ALREADY_INITIALIZED;
	if (client->_handshake_initialized) return AC_STATUS_ALREADY_HANDSHAKED;

	if (!ac_wsa_initialized) {
		const int wsa_startup_status = WSAStartup(MAKEWORD(2, 2), &ac_wsa_data);
		if (wsa_startup_status != 0) {
			fprintf(stderr, "AC: WSAStartup failed. Error: %d; %d\n", WSAGetLastError(), wsa_startup_status);
			return AC_STATUS_SOCKET_ERROR;
		}
	}

	ac_wsa_initialized = true;
	ac_wsa_usage_count++;

	client->_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (client->_socket == INVALID_SOCKET) {
		fprintf(stderr, "AC: socket failed. Error: %d\n", WSAGetLastError());
		return AC_STATUS_SOCKET_ERROR;
	}

	client->_socket_initialized = true;

	ZeroMemory(&client->_server_address, ac_server_address_size);
	client->_server_address.sin_family = AF_INET;
	client->_server_address.sin_port = htons(AC_SERVER_PORT);

	if (inet_pton(AF_INET, AC_SERVER_IP, &client->_server_address.sin_addr) != 1) {
		fprintf(stderr, "AC: inet_pton failed. Error: %d\n", WSAGetLastError());
		return AC_STATUS_SOCKET_ERROR;
	}

	client->_address_initialized = true;
	return AC_STATUS_OK;
}

ac_status_t ac_client_close(ac_client_t *client) {
	if (client->_handshake_initialized) return AC_STATUS_NOT_DISMISSED;

	if (client->_address_initialized) {
		ZeroMemory(&client->_server_address, ac_server_address_size);
		client->_address_initialized = false;
	}

	if (client->_socket_initialized) {
		closesocket(client->_socket);
		client->_socket = INVALID_SOCKET;
		client->_socket_initialized = false;
	}

	if (ac_wsa_initialized) {
		ac_wsa_usage_count--;
		if (ac_wsa_usage_count == 0) {
			WSACleanup();
			ZeroMemory(&ac_wsa_data, sizeof(ac_wsa_data));
			ac_wsa_initialized = false;
		}
	}

	return AC_STATUS_OK;
}

ac_status_t ac_client_send(ac_client_t *client, const ac_request_t request) {
	if (!ac_wsa_initialized) return AC_STATUS_NOT_INITIALIZED;
	if (!client->_socket_initialized) return AC_STATUS_NOT_INITIALIZED;
	if (!client->_address_initialized) return AC_STATUS_NOT_INITIALIZED;
	if (!client->_handshake_initialized && request.operation != AC_OPERATION_HANDSHAKE) return AC_STATUS_NOT_HANDSHAKED;

	const int sent = sendto(client->_socket, (const char*)&request, ac_request_size, 0, (struct sockaddr *)&client->_server_address, ac_server_address_size);
	if (sent == SOCKET_ERROR) {
		fprintf(stderr, "AC: sendto failed. Error: %d\n", WSAGetLastError());
		return AC_STATUS_SEND_ERROR;
	}

	if (request.operation == AC_OPERATION_HANDSHAKE) client->_handshake_initialized = true;
	if (request.operation == AC_OPERATION_DISMISS) client->_handshake_initialized = false;
	return AC_STATUS_OK;
}

ac_status_t ac_client_handshake(ac_client_t *client) {
	const ac_request_t request = {
		.identifier = AC_IDENTIFIER,
		.version = AC_SERVER_VERSION,
		.operation = AC_OPERATION_HANDSHAKE,
	};

	return ac_client_send(client, request);
}

ac_status_t ac_client_subscribe_update(ac_client_t *client) {
	const ac_request_t request = {
		.identifier = AC_IDENTIFIER,
		.version = AC_SERVER_VERSION,
		.operation = AC_OPERATION_SUBSCRIBE_UPDATE,
	};

	return ac_client_send(client, request);
}

ac_status_t ac_client_subscribe_spot(ac_client_t *client) {
	const ac_request_t request = {
		.identifier = AC_IDENTIFIER,
		.version = AC_SERVER_VERSION,
		.operation = AC_OPERATION_SUBSCRIBE_SPOT,
	};

	return ac_client_send(client, request);
}

ac_status_t ac_client_dismiss(ac_client_t *client) {
	const ac_request_t request = {
		.identifier = AC_IDENTIFIER,
		.version = AC_SERVER_VERSION,
		.operation = AC_OPERATION_DISMISS,
	};

	return ac_client_send(client, request);
}

ac_status_t ac_client_receive(ac_client_t *client, char buffer[AC_BUFFER_SIZE], ac_event_t *event, ac_event_type_t *event_type) {
	if (!ac_wsa_initialized) return AC_STATUS_NOT_INITIALIZED;
	if (!client->_socket_initialized) return AC_STATUS_NOT_INITIALIZED;
	if (!client->_address_initialized) return AC_STATUS_NOT_INITIALIZED;
	if (!client->_handshake_initialized) return AC_STATUS_NOT_HANDSHAKED;

	const int received = recvfrom(client->_socket, buffer, AC_BUFFER_SIZE, 0, (struct sockaddr *)&client->_server_address, &ac_server_address_size);
	if (received == SOCKET_ERROR) {
		const int err = WSAGetLastError();
		if (err == WSAETIMEDOUT) {
			fprintf(stderr, "AC: recvfrom failed (timeout)\n");
			return AC_STATUS_TIMEOUT_ERROR;
		} else {
			fprintf(stderr, "AC: recvfrom failed. Error: %d\n", err);
			return AC_STATUS_RECEIVE_ERROR;
		}
	}

	switch (received) {
		case sizeof(ac_response_t):
			*event_type = AC_EVENT_TYPE_HANDSHAKE;
			event->handshake = (ac_response_t *)buffer;
			break;
		case sizeof(ac_rt_car_info):
			*event_type = AC_EVENT_TYPE_CAR_INFO;
			event->car_info = (ac_rt_car_info *)buffer;
			break;
		case sizeof(ac_rt_lap_info):
			*event_type = AC_EVENT_TYPE_LAP_INFO;
			event->lap_info = (ac_rt_lap_info *)buffer;
			break;
		default:
			fprintf(stderr, "AC: recvfrom received unexpected amount of data. Size: %d\n", received);
			return AC_STATUS_RECEIVED_INVALID_DATA;
	}

	return AC_STATUS_OK;
}