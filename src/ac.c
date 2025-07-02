#include <stdio.h>
#include <stdint.h>

#include "../include/libracing/ac.h"

ac_client_t* ac_client_create() {
	ac_client_t* client = malloc(sizeof(ac_client_t));
	if (client == NULL) {
		ac_client_close(client);
		return NULL;
	}

	client->udp_socket = udp_socket_create(AC_SERVER_IP, AC_SERVER_PORT);
	if (client->udp_socket == NULL) {
		ac_client_close(client);
		return NULL;
	}

	return client;
}

ac_status_t ac_client_close(ac_client_t *client) {
	if (client == NULL) {
		return AC_STATUS_CLIENT_NOT_INITIALIZED;
	}

	if (client->udp_socket != NULL) {
		udp_socket_close(client->udp_socket);
	}

	free(client);
	return AC_STATUS_OK;
}

ac_status_t ac_client_send(ac_client_t *client, const ac_operation_t operation) {
	if (client == NULL) return AC_STATUS_CLIENT_NOT_INITIALIZED;
	if (client->_handshake_initialized == false && operation != AC_OPERATION_HANDSHAKE) return AC_STATUS_NOT_HANDSHAKED;

	const ac_request_t request = ac_request_t{
		.identifier = AC_IDENTIFIER,
		.version = AC_SERVER_VERSION,
		.operation = operation,
	};

	const int sent = udp_socket_send(client->udp_socket, (const char*)&request, sizeof(ac_request_t));
	if (sent < 0) {
		fprintf(stderr, "AC: udp_socket_send failed\n");
		return AC_STATUS_SEND_ERROR;
	}

	if (operation == AC_OPERATION_HANDSHAKE) client->_handshake_initialized = true;
	if (operation == AC_OPERATION_DISMISS) client->_handshake_initialized = false;

	return AC_STATUS_OK;
}

ac_status_t ac_client_receive(ac_client_t *client, char buffer[AC_BUFFER_SIZE], ac_event_t *event, ac_event_type_t *event_type) {
	if (client == NULL) return AC_STATUS_NOT_INITIALIZED;
	if (client->_handshake_initialized == false) return AC_STATUS_NOT_HANDSHAKED;

	const int received = udp_socket_receive(client->udp_socket, buffer, AC_BUFFER_SIZE);
	if (received < 0) {
		fprintf(stderr, "AC: udp_socket_receive failed");
		return AC_STATUS_RECEIVE_ERROR;
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
			fprintf(stderr, "AC: udp_socket_receive received unexpected amount of data. Size: %d\n", received);
			return AC_STATUS_RECEIVED_INVALID_DATA;
	}

	return AC_STATUS_OK;
}

ac_status_t ac_client_handshake(ac_client_t *client) { return ac_client_send(client, AC_OPERATION_HANDSHAKE); }
ac_status_t ac_client_subscribe_update(ac_client_t *client) { return ac_client_send(client, AC_OPERATION_SUBSCRIBE_UPDATE); }
ac_status_t ac_client_subscribe_spot(ac_client_t *client) { return ac_client_send(client, AC_OPERATION_SUBSCRIBE_SPOT); }
ac_status_t ac_client_dismiss(ac_client_t *client) { return ac_client_send(client, AC_OPERATION_DISMISS); }