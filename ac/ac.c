#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ac.h"

#pragma comment(lib, "Ws2_32.lib")

static WSADATA *ac_wsa_data = NULL;
static SOCKET ac_socket = INVALID_SOCKET;
static struct sockaddr_in ac_server_address;
static int ac_server_address_size = sizeof(ac_server_address);

static bool ac_initialized = false;
static bool ac_handshaked = false;

ac_status_t ac_init() {
	if (ac_initialized) {
		return AC_STATUS_ALREADY_INITIALIZED;
	}

	ZeroMemory(&ac_server_address, sizeof(ac_server_address));
	ac_server_address.sin_family = AF_INET;
	ac_server_address.sin_port = htons(AC_SERVER_PORT);

	if (inet_pton(AF_INET, AC_SERVER_IP, &ac_server_address.sin_addr) != 1) {
		fprintf(stderr, "AC: inet_pton failed. Error: %d\n", WSAGetLastError());
		return AC_STATUS_SOCKET_ERROR;
	}

	if (WSAStartup(MAKEWORD(2, 2), ac_wsa_data) != 0) {
		fprintf(stderr, "AC: WSAStartup failed. Error: %d\n", WSAGetLastError());
		return AC_STATUS_SOCKET_ERROR;
	}

	ac_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (ac_socket == INVALID_SOCKET) {
        fprintf(stderr, "AC: socket failed. Error: %d\n", WSAGetLastError());
        return AC_STATUS_SOCKET_ERROR;
	}

	ac_initialized = true;
	return true;
}

ac_status_t ac_close() {
	if (ac_socket != INVALID_SOCKET) {
		closesocket(ac_socket);
		ac_socket = INVALID_SOCKET;
	}

	if (ac_wsa_data != NULL) {
		WSACleanup();
		ac_wsa_data = NULL;
	}

	ac_handshaked = false;
	ac_initialized = false;
	return AC_STATUS_OK;
}

ac_status_t ac_send(const ac_handshaker_request_t request) {
	if (!ac_initialized) {
		return AC_STATUS_NOT_INITIALIZED;
	}

	if (!ac_handshaked && request.operation != AC_OPERATION_HANDSHAKE) {
		return AC_STATUS_NOT_HANDSHAKED;
	}

	const int sent = sendto(ac_socket, (const char*)&request, sizeof(ac_handshaker_request_t), 0, (struct sockaddr *)&ac_server_address, ac_server_address_size);
	if (sent == SOCKET_ERROR) {
		fprintf(stderr, "AC: sendto failed. Error: %d\n", WSAGetLastError());
		return AC_STATUS_SEND_ERROR;
	}

	switch (request.operation) {
		case AC_OPERATION_HANDSHAKE:
			ac_handshaked = true;
			break;
		case AC_OPERATION_DISMISS:
			ac_handshaked = false;
			break;
	}

	return AC_STATUS_OK;
}

ac_status_t ac_receive(char *buffer, const int buffer_size, ac_event_t *event, ac_event_type_t *event_type) {
	const int received = recvfrom(ac_socket, buffer, buffer_size, 0, (struct sockaddr *)&ac_server_address, &ac_server_address_size);
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
		case sizeof(ac_handshaker_request_t):
			*event_type = AC_EVENT_TYPE_HANDSHAKE;
			event->handshake = (ac_handshaker_response_t *)buffer;
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
