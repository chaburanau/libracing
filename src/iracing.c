#include <stddef.h>
#include <windows.h>

#include "../include/libracing/iracing.h"


#define IRACING_MEM_MAP_FILE_NAME "Local\\IRSDKMemMapFileName"


typedef struct iRacingClient {
	HANDLE file_handle;
	char *shared_memory;
	iracing_header_t header;
} iracing_client_t;

iracing_client_t *iracing_connect() {
	iracing_client_t *client = malloc(sizeof(iracing_client_t));
	if (client == NULL) {
		return NULL;
	}

	client->file_handle = OpenFileMapping(FILE_MAP_READ, FALSE, IRACING_MEM_MAP_FILE_NAME);
	if (client->file_handle == NULL) {
		iracing_disconnect(client);
		return NULL;
	}

	client->shared_memory = (char*)MapViewOfFile(client->file_handle, FILE_MAP_READ, 0, 0, 0);
	if (client->file_handle == NULL) {
		iracing_disconnect(client);
		return NULL;
	}

	return client;
}

void iracing_disconnect(iracing_client_t *client) {
	if (client == NULL) {
		return;
	}

	if (client->shared_memory != NULL) {
		UnmapViewOfFile(client->shared_memory);
		client->shared_memory = NULL;
	}

	if (client->file_handle != NULL) {
		CloseHandle(client->file_handle);
		client->file_handle = NULL;
	}

	free(client);
}

iracing_header_t iracing_get_header(iracing_client_t *client) {
	iracing_header_t header;
	if (client == NULL) { return header; }

	memcpy(&header, client->shared_memory, sizeof(iracing_header_t));
	client->header = header;
	return header;
}

iracing_session_info_t iracing_get_session_info(iracing_client_t *client) {
	iracing_session_info_t session_info;
	if (client == NULL) { return session_info; }

	int32_t offset = client->header.session_info_offset;
	memcpy(&session_info, client->shared_memory + offset, sizeof(iracing_session_info_t));
	return session_info;
}

iracing_variable_header_t iracing_get_variable_header(iracing_client_t *client, int32_t index) {
	iracing_variable_header_t variable_header;
	if (client == NULL) { return variable_header; }

	int32_t offset = client->header.variables_header_offset + index * sizeof(iracing_variable_header_t);
	memcpy(&variable_header, client->shared_memory + offset, sizeof(iracing_variable_header_t));
	return variable_header;
}

char iracing_get_char_variable(iracing_client_t *client, int offset) {
	char data;
	memcpy(&data, client->shared_memory + offset, sizeof(char));
	return data;
}

bool iracing_get_bool_variable(iracing_client_t *client, int offset) {
	bool data;
	memcpy(&data, client->shared_memory + offset, sizeof(bool));
	return data;
}

int32_t iracing_get_int_variable(iracing_client_t *client, int offset) {
	int32_t data;
	memcpy(&data, client->shared_memory + offset, sizeof(int32_t));
	return data;
}

int32_t iracing_get_bitfield_variable(iracing_client_t *client, int offset) {
	int32_t data;
	memcpy(&data, client->shared_memory + offset, sizeof(int32_t));
	return data;
}

float iracing_get_float_variable(iracing_client_t *client, int offset) {
	float data;
	memcpy(&data, client->shared_memory + offset, sizeof(float));
	return data;
}

double iracing_get_double_variable(iracing_client_t *client, int offset) {
	double data;
	memcpy(&data, client->shared_memory + offset, sizeof(double));
	return data;
}
