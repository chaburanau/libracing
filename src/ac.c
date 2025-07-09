#include "../include/libracing/ac.h"

#define AC_BUFFER_SIZE 408

ac_result_t ac_client_connect(ac_client_t *client) {
	ac_result_t result;

	if (client == NULL) {
		result.status = AC_STATUS_CLIENT_NOT_INITIALIZED;
		return result;
	}

	client->_udp_socket = malloc(sizeof(udp_socket_t));
	if (client->_udp_socket == NULL) {
		result.status = AC_STATUS_ALLOCATION_FAILED;
		return result;
	}

	udp_socket_init_status_t status = udp_socket_connect(client->_udp_socket, AC_SERVER_IP, AC_SERVER_PORT);
	if (status.last_error != 0) {
		result.status = AC_STATUS_SOCKET_ERROR;
		result.underlying_error = status.last_error;
		return result;
	}

	result.status = AC_STATUS_OK;
	return result;
}

ac_result_t ac_client_close(ac_client_t *client) {
	ac_result_t result;

	if (client == NULL) {
		result.status = AC_STATUS_CLIENT_NOT_INITIALIZED;
		return result;
	}

	if (client->_udp_socket != NULL) {
		udp_socket_init_status_t status = udp_socket_close(client->_udp_socket);
		free(client->_udp_socket);
		client->_udp_socket = NULL;

		if (status.last_error != 0) {
			result.status = AC_STATUS_SOCKET_ERROR;
			result.underlying_error = status.last_error;
			return result;
		}
	}

	result.status = AC_STATUS_OK;
	return result;
}

ac_result_t ac_client_send(ac_client_t *client, const ac_operation_t operation) {
	ac_result_t result;

	if (client == NULL) {
		result.status = AC_STATUS_CLIENT_NOT_INITIALIZED;
		return result;
	}

	if (client->_handshake_performed == false && operation != AC_OPERATION_HANDSHAKE) {
		result.status = AC_STATUS_HANDSHAKE_NOT_PERFORMED;
		return result;
	}

	const ac_request_t request = {
		.identifier = AC_IDENTIFIER,
		.version = AC_SERVER_VERSION,
		.operation = operation,
	};

	udp_socket_io_status_t status = udp_socket_send(client->_udp_socket, (const char*)&request, sizeof(ac_request_t));
	if (status.last_error != 0) {
		result.status = AC_STATUS_SEND_ERROR;
		result.underlying_error = status.last_error;
		return result;
	}

	if (operation == AC_OPERATION_HANDSHAKE) { client->_handshake_performed = true; }
	if (operation == AC_OPERATION_DISMISS) { client->_handshake_performed = false; }

	result.status = AC_STATUS_OK;
	return result;
}

ac_result_t ac_client_receive(ac_client_t *client, ac_event_t *event) {
	ac_result_t result;

	if (client == NULL) {
		result.status = AC_STATUS_CLIENT_NOT_INITIALIZED;
		return result;
	}

	if (client->_handshake_performed == false) {
		result.status = AC_STATUS_HANDSHAKE_NOT_PERFORMED;
		return result;
	}

	char *buffer = malloc(AC_BUFFER_SIZE);
	if (buffer == NULL) {
		result.status = AC_STATUS_ALLOCATION_FAILED;
		return result;
	}

	udp_socket_io_status_t status = udp_socket_receive(client->_udp_socket, buffer, AC_BUFFER_SIZE);
	if (status.last_error != 0) {
		result.status = AC_STATUS_RECEIVE_ERROR;
		result.underlying_error = status.last_error;
		return result;
	}

	switch (status.bytes_received) {
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
			result.status = AC_STATUS_RECEIVED_INVALID_DATA;
			result.underlying_error = status.bytes_received;
			return result;
	}

	result.status = AC_STATUS_OK;
	return result;
}

ac_result_t ac_client_handshake(ac_client_t *client) { return ac_client_send(client, AC_OPERATION_HANDSHAKE); }
ac_result_t ac_client_subscribe_update(ac_client_t *client) { return ac_client_send(client, AC_OPERATION_SUBSCRIBE_UPDATE); }
ac_result_t ac_client_subscribe_spot(ac_client_t *client) { return ac_client_send(client, AC_OPERATION_SUBSCRIBE_SPOT); }
ac_result_t ac_client_dismiss(ac_client_t *client) { return ac_client_send(client, AC_OPERATION_DISMISS); }
