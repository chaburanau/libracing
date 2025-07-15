#include <stdlib.h>

#include "../include/libracing/udp_socket.h"
#include "../include/libracing/ac.h"

#define AC_BUFFER_SIZE 408

typedef struct AssettoCorsaClient {
	udp_socket_t* _udp_socket;
	bool _handshake_performed;
} ac_client_t;

ac_client_t *ac_client_create(const char *address, int port) {
	ac_client_t *client = malloc(sizeof(ac_client_t));
	if (client == NULL) {
		return NULL;
	}

	udp_socket_t *udp_socket = udp_socket_create(address, port);
	if (udp_socket == NULL) {
		free(client);
		return NULL;
	}

	return client;
}

void ac_client_destroy(ac_client_t *client) {
	if (client == NULL) { return; }
	if (client->_udp_socket != NULL) { udp_socket_destroy(client->_udp_socket); }
	free(client);
}

ac_status_t ac_client_send(ac_client_t *client, const ac_operation_t operation) {
	if (client == NULL) { return AC_STATUS_CLIENT_NOT_INITIALIZED; }
	if (client->_handshake_performed == false && operation != AC_OPERATION_HANDSHAKE) { return AC_STATUS_HANDSHAKE_NOT_PERFORMED; }

	const ac_request_t request = {
		.identifier = AC_IDENTIFIER,
		.version = AC_SERVER_VERSION,
		.operation = operation,
	};

	int32_t bytes_send = udp_socket_send(client->_udp_socket, (char*)&request, sizeof(ac_request_t));
	if (bytes_send < 0) { return AC_STATUS_SEND_ERROR; }

	if (operation == AC_OPERATION_HANDSHAKE) { client->_handshake_performed = true; }
	if (operation == AC_OPERATION_DISMISS) { client->_handshake_performed = false; }

	return AC_STATUS_OK;
}

ac_status_t ac_client_receive(ac_client_t *client, ac_event_t *event) {
	if (client == NULL) { return AC_STATUS_CLIENT_NOT_INITIALIZED; }
	if (client->_handshake_performed == false) { return AC_STATUS_HANDSHAKE_NOT_PERFORMED; }

	char *buffer = malloc(AC_BUFFER_SIZE);
	if (buffer == NULL) { return AC_STATUS_ALLOCATION_FAILED; }

	int32_t bytes_received = udp_socket_receive(client->_udp_socket, buffer, AC_BUFFER_SIZE);
	if (bytes_received < 0) { return AC_STATUS_RECEIVE_ERROR; }

	switch (bytes_received) {
		case sizeof(ac_response_t):
			event->type = AC_EVENT_TYPE_HANDSHAKE;
			event->data.handshake = (ac_response_t *)buffer;
			break;
		case sizeof(ac_rt_car_info):
			event->type = AC_EVENT_TYPE_CAR_INFO;
			event->data.car_info = (ac_rt_car_info *)buffer;
			break;
		case sizeof(ac_rt_lap_info):
			event->type = AC_EVENT_TYPE_LAP_INFO;
			event->data.lap_info = (ac_rt_lap_info *)buffer;
			break;
		default:
			return AC_STATUS_RECEIVED_INVALID_DATA;
	}

	return AC_STATUS_OK;
}

ac_status_t ac_client_handshake(ac_client_t *client) { return ac_client_send(client, AC_OPERATION_HANDSHAKE); }
ac_status_t ac_client_subscribe_update(ac_client_t *client) { return ac_client_send(client, AC_OPERATION_SUBSCRIBE_UPDATE); }
ac_status_t ac_client_subscribe_spot(ac_client_t *client) { return ac_client_send(client, AC_OPERATION_SUBSCRIBE_SPOT); }
ac_status_t ac_client_dismiss(ac_client_t *client) { return ac_client_send(client, AC_OPERATION_DISMISS); }
