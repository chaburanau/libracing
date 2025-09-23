#include <stddef.h>
#include <stdlib.h>
#include <windows.h>

#include "./types.c"
#include "./utils.c"

static int32_t iracing_last_error = 0;
int32_t iracing_get_last_error(void) { return iracing_last_error; }

typedef struct {
    HANDLE file_handle;
    HANDLE event_handle;
    char *memory;
} iracing_client_t;

// --

bool iracing_client_init(iracing_client_t *client);
bool iracing_client_free(iracing_client_t *client);

bool iracing_read_header(iracing_client_t *client, iracing_header_t *header);
bool iracing_read_session_info(iracing_client_t *client, iracing_header_t *header, iracing_session_info_t *session_info);
bool iracing_read_variable_info(iracing_client_t *client, iracing_header_t *header, iracing_variable_info_t *info, size_t index);
bool iracing_read_variable_value(iracing_client_t *client, iracing_header_t *header, iracing_variable_info_t *info, void *value);

// --

bool iracing_client_init(iracing_client_t *client) {
    if (client == NULL) {
        iracing_last_error = IRACING_STATUS_CLIENT_NOT_INITIALIZED;
        return false;
    }

    client->file_handle = OpenFileMapping(FILE_MAP_READ, FALSE, IRACING_MEM_MAP_FILE_NAME);
    if (client->file_handle == NULL) {
        iracing_client_free(client);
        iracing_last_error = (int32_t)GetLastError();
        return false;
    }

    client->event_handle = OpenEvent(SYNCHRONIZE, false, IRACING_DATA_VALID_EVENT_NAME);
    if (client->event_handle == NULL) {
        iracing_client_free(client);
        iracing_last_error = (int32_t)GetLastError();
        return false;
    }

    client->memory = (char *)MapViewOfFile(client->file_handle, FILE_MAP_READ, 0, 0, 0);
    if (client->memory == NULL) {
        iracing_client_free(client);
        iracing_last_error = (int32_t)GetLastError();
        return false;
    }

    return true;
}

bool iracing_client_free(iracing_client_t *client) {
    if (client == NULL) {
        iracing_last_error = IRACING_STATUS_CLIENT_NOT_INITIALIZED;
        return false;
    }

    if (client->memory != NULL) {
        bool success = UnmapViewOfFile(client->memory);
        if (!success) {
            iracing_last_error = (int32_t)GetLastError();
        }
    }

    if (client->event_handle != NULL) {
        bool success = CloseHandle(client->event_handle);
        if (!success) {
            iracing_last_error = (int32_t)GetLastError();
        }
    }

    if (client->file_handle != NULL) {
        bool success = CloseHandle(client->file_handle);
        if (!success) {
            iracing_last_error = (int32_t)GetLastError();
        }
    }

    client->memory = NULL;
    client->event_handle = NULL;
    client->file_handle = NULL;

    return true;
}

bool iracing_read_header(iracing_client_t *client, iracing_header_t *header) {
    if (client == NULL) {
        iracing_last_error = IRACING_STATUS_CLIENT_NOT_INITIALIZED;
        return false;
    }

    memcpy(header, client->memory, sizeof(iracing_header_t));
    return true;
}

bool iracing_read_session_info(iracing_client_t *client, iracing_header_t *header, iracing_session_info_t *session_info) {
    if (client == NULL) {
        iracing_last_error = IRACING_STATUS_CLIENT_NOT_INITIALIZED;
        return false;
    }
    if (header == NULL) {
        iracing_last_error = IRACING_STATUS_HEADER_NOT_INITIALIZED;
        return false;
    }

    int32_t version = header->session_info_version;
    int32_t offset = header->session_info_offset;
    int32_t length = header->session_info_length;

    session_info->version = version;
    session_info->data.size = (size_t)length;
    session_info->data.data = malloc((size_t)length);

    if (session_info->data.data == NULL) {
        iracing_last_error = IRACING_STATUS_ALLOCATION_FAILED;
        return false;
    }

    memcpy(session_info->data.data, client->memory + offset, (size_t)length);
    return true;
}

bool iracing_read_variable_info(iracing_client_t *client, iracing_header_t *header, iracing_variable_info_t *info, size_t index) {
    if (client == NULL) {
        iracing_last_error = IRACING_STATUS_CLIENT_NOT_INITIALIZED;
        return false;
    }
    if (header == NULL) {
        iracing_last_error = IRACING_STATUS_HEADER_NOT_INITIALIZED;
        return false;
    }
    if (info == NULL) {
        iracing_last_error = IRACING_STATUS_INFO_NOT_INITIALIZED;
        return false;
    }
    if (index >= (size_t)header->number_of_variables) {
        iracing_last_error = IRACING_STATUS_ALLOCATION_FAILED;
        return false;
    }

    size_t length = sizeof(iracing_variable_info_t);
    size_t offset = (size_t)header->variables_info_offset + (length * index);

    memcpy(info, client->memory + offset, length);
    return true;
}

bool iracing_read_variable_value(iracing_client_t *client, iracing_header_t *header, iracing_variable_info_t *info, void *value) {
    if (client == NULL) {
        iracing_last_error = IRACING_STATUS_CLIENT_NOT_INITIALIZED;
        return false;
    }
    if (header == NULL) {
        iracing_last_error = IRACING_STATUS_HEADER_NOT_INITIALIZED;
        return false;
    }
    if (info == NULL) {
        iracing_last_error = IRACING_STATUS_INFO_NOT_INITIALIZED;
        return false;
    }

    size_t index = iracing_get_latest_tick(header);
    size_t length = iracing_variable_size(info->type) * (size_t)info->count;
    size_t offset = (size_t)(header->variable_buffers[index].offset + info->offset);

    value = malloc(length);
    if (value == NULL) {
        iracing_last_error = IRACING_STATUS_ALLOCATION_FAILED;
        return false;
    }

    memcpy(value, client->memory + offset, length);
    return true;
}

// --
