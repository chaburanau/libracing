#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ac.h"

#pragma comment(lib, "Ws2_32.lib")

#define HANDSHAKE_COUNT 3      // Number of handshake messages to send
#define BUFFER_SIZE 1024       // Receive buffer size
#define TIMEOUT_MILLISECONDS 2000  // Timeout for recv in ms

static bool initialized = false;
static bool handshaked = false;

WSADATA *wsaData;
SOCKET sock = INVALID_SOCKET;
struct sockaddr_in server_addr;

bool ac_init() {
	if (initialized) {
		return true;
	}

	if (WSAStartup(MAKEWORD(2, 2), wsaData) != 0) {
		fprintf(stderr, "WSAStartup failed. Error: %d\n", WSAGetLastError());
		ac_close();
		return false;
	}

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET) {
        fprintf(stderr, "socket() failed. Error: %d\n", WSAGetLastError());
		ac_close();
        return false;
	}

    ZeroMemory(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(AC_SERVER_PORT);

    if (inet_pton(AF_INET, AC_SERVER_IP, &server_addr.sin_addr) != 1) {
        fprintf(stderr, "inet_pton() failed. Error: %d\n", WSAGetLastError());
		ac_close();
        return false;
    }

    DWORD timeout = TIMEOUT_MILLISECONDS;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) == SOCKET_ERROR) {
        fprintf(stderr, "setsockopt(SO_RCVTIMEO) failed. Error: %d\n", WSAGetLastError());
		ac_close();
        return false;
    }

	initialized = true;
	return true;
}

bool ac_handshake() {
	if (!initialized) {
		return false;
	}

	if (handshaked) {
		return true;
	}

	ac_handshaker_request_t request = {
		.identifier = AC_IDENTIFIER,
		.version = AC_SERVER_VERSION,
		.operation = AC_OPERATION_HANDSHAKE,
	};
	ac_handshaker_response_t response;

	int sent = sendto(sock, (const char*)&request, sizeof(request), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (sent == SOCKET_ERROR) {
		fprintf(stderr, "sendto() failed. Error: %d\n", WSAGetLastError());
		ac_close();
		return false;
	}

	int received = recvfrom(sock, (char*)&response, sizeof(response), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (received == SOCKET_ERROR) {
        int err = WSAGetLastError();
        if (err == WSAETIMEDOUT) {
            fprintf(stderr, "Handshake: no ACK (timeout), retrying...\n");
			ac_close();
			return false;
        } else {
            fprintf(stderr, "recvfrom() failed. Error: %d\n", err);
			ac_close();
            return false;
        }
    }

	handshaked = true;
	return true;
}

bool ac_subscribe_update(void (*callback)(ac_rt_car_info)) {
	return false;
}

bool ac_subscribe_spot(void (*callback)(ac_rt_lap_info)) {
	return false;
}

bool ac_dismiss() {
	return false;
}

void ac_close() {
	if (sock != INVALID_SOCKET) {
        closesocket(sock);
	}

	if (wsaData != NULL) {
        WSACleanup();
	}

	handshaked = false;
	initialized = false;
}