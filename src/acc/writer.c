#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/libracing/acc.h"
#include "../../include/libracing/types.h"

void acc_write_type(char *buffer, size_t *offset, void *data, size_t size) {
    memcpy(buffer + *offset, data, size);
    (*offset) += size;
}

void acc_write_int8_t(char *buffer, size_t *offset, int8_t data) {
    acc_write_type(buffer, offset, &data, sizeof(int8_t));
}

void acc_write_int32_t(char *buffer, size_t *offset, int32_t data) {
    acc_write_type(buffer, offset, &data, sizeof(int32_t));
}

void acc_write_uint16_t(char *buffer, size_t *offset, uint16_t data) {
    acc_write_type(buffer, offset, &data, sizeof(uint16_t));
}

void acc_write_float(char *buffer, size_t *offset, float data) {
    acc_write_type(buffer, offset, &data, sizeof(float));
}

void acc_write_string(char *buffer, size_t *offset, string_t *string) {
    if (string == NULL || string->size == 0) {
        acc_write_uint16_t(buffer, offset, 0);
    } else {
        acc_write_uint16_t(buffer, offset, (uint16_t)string->size);
        acc_write_type(buffer, offset, string->data, string->size);
    }
}

int32_t acc_write_register_application(char **buffer, size_t *total, acc_reg_app_t *reg_app) {
    (*total) += sizeof(int8_t);                         // 1. Request Type
    (*total) += sizeof(int8_t);                         // 2. Broadcasting Version
    (*total) += reg_app->display_name->size + 2;        // 3. Display Name
    (*total) += reg_app->connection_password->size + 2; // 4. Connection Password
    (*total) += sizeof(int32_t);                        // 5. Update Interval
    (*total) += reg_app->command_password->size + 2;    // 6. Command Password

    *buffer = malloc(*total);
    if (*buffer == NULL) {
        return ACC_STATUS_MEMORY_ALLOCATION_ERROR;
    }

    size_t offset = 0;
    acc_write_int8_t(*buffer, &offset, ACC_OUTBOUND_MESSAGE_REGISTER_COMMAND_APPLICATION);
    acc_write_int8_t(*buffer, &offset, ACC_BROADCASTING_PROTOCOL_VERSION);
    acc_write_string(*buffer, &offset, reg_app->display_name);
    acc_write_string(*buffer, &offset, reg_app->connection_password);
    acc_write_int32_t(*buffer, &offset, reg_app->update_interval);
    acc_write_string(*buffer, &offset, reg_app->command_password);

    if (*total != offset) {
        return ACC_STATUS_BUFFER_SIZE_MISMATCH;
    }

    return ACC_STATUS_OK;
}

int32_t acc_write_unregister_application(char **buffer, size_t *total, acc_unreg_app_t *unreg_app) {
    (void)unreg_app;

    (*total) += sizeof(int8_t); // 1. Request Type

    *buffer = malloc(*total);
    if (*buffer == NULL) {
        return ACC_STATUS_MEMORY_ALLOCATION_ERROR;
    }

    size_t offset = 0;
    acc_write_int8_t(*buffer, &offset, ACC_OUTBOUND_MESSAGE_UNREGISTER_COMMAND_APPLICATION);

    if (*total != offset) {
        return ACC_STATUS_BUFFER_SIZE_MISMATCH;
    }

    return ACC_STATUS_OK;
}

int32_t acc_write_request_entry_list(char **buffer, size_t *total, acc_req_entry_list_t *req_entry_list) {
    (*total) += sizeof(int8_t);  // 1. Request Type
    (*total) += sizeof(int32_t); // 2. Connection ID

    *buffer = malloc(*total);
    if (*buffer == NULL) {
        return ACC_STATUS_MEMORY_ALLOCATION_ERROR;
    }

    size_t offset = 0;
    acc_write_int8_t(*buffer, &offset, ACC_OUTBOUND_MESSAGE_REQUEST_ENTRY_LIST);
    acc_write_int32_t(*buffer, &offset, req_entry_list->connection_id);

    if (*total != offset) {
        return ACC_STATUS_BUFFER_SIZE_MISMATCH;
    }

    return ACC_STATUS_OK;
}

int32_t acc_write_request_track_data(char **buffer, size_t *total, acc_req_track_data_t *req_track_data) {
    (*total) += sizeof(int8_t);  // 1. Request Type
    (*total) += sizeof(int32_t); // 2. Connection ID

    *buffer = malloc(*total);
    if (*buffer == NULL) {
        return ACC_STATUS_MEMORY_ALLOCATION_ERROR;
    }

    size_t offset = 0;
    acc_write_int8_t(*buffer, &offset, ACC_OUTBOUND_MESSAGE_REQUEST_TRACK_DATA);
    acc_write_int32_t(*buffer, &offset, req_track_data->connection_id);

    if (*total != offset) {
        return ACC_STATUS_BUFFER_SIZE_MISMATCH;
    }

    return ACC_STATUS_OK;
}

int32_t acc_write_change_hud_page(char **buffer, size_t *total, acc_change_hud_page_t *change_hud_page) {
    (*total) += sizeof(int8_t);                      // 1. Request Type
    (*total) += sizeof(int32_t);                     // 2. Connection ID
    (*total) += change_hud_page->hud_page->size + 2; // 3. HUD Page

    *buffer = malloc(*total);
    if (*buffer == NULL) {
        return ACC_STATUS_MEMORY_ALLOCATION_ERROR;
    }

    size_t offset = 0;
    acc_write_int8_t(*buffer, &offset, ACC_OUTBOUND_MESSAGE_CHANGE_HUD_PAGE);
    acc_write_int32_t(*buffer, &offset, change_hud_page->connection_id);
    acc_write_string(*buffer, &offset, change_hud_page->hud_page);

    if (*total != offset) {
        return ACC_STATUS_BUFFER_SIZE_MISMATCH;
    }

    return ACC_STATUS_OK;
}

int32_t acc_write_change_focus(char **buffer, size_t *total, acc_change_focus_t *change_focus) {
    bool has_car_index = change_focus->car_index != NULL;
    bool has_cameras = change_focus->camera_set != NULL || change_focus->camera != NULL;

    (*total) += sizeof(int8_t);  // 1. Request Type
    (*total) += sizeof(int32_t); // 2. Connection ID
    (*total) += sizeof(int8_t);  // 3.1. Car Index Indicator
    if (has_car_index) {
        (*total) += sizeof(uint16_t); // 3.2. Car Index
    }

    (*total) += sizeof(int8_t); // 4.1. Camera Indicator
    if (has_cameras) {
        (*total) += change_focus->camera_set->size + 2; // 4.2. Camera Set
    }
    if (has_cameras) {
        (*total) += change_focus->camera->size + 2; // 4.3. Camera
    }

    *buffer = malloc(*total);
    if (*buffer == NULL) {
        return ACC_STATUS_MEMORY_ALLOCATION_ERROR;
    }

    size_t offset = 0;
    acc_write_int8_t(*buffer, &offset, ACC_OUTBOUND_MESSAGE_CHANGE_FOCUS);
    acc_write_int32_t(*buffer, &offset, change_focus->connection_id);

    acc_write_int8_t(*buffer, &offset, has_car_index ? 1 : 0);
    if (has_car_index) {
        acc_write_uint16_t(*buffer, &offset, *change_focus->car_index);
    }

    acc_write_int8_t(*buffer, &offset, has_cameras ? 1 : 0);
    if (has_cameras) {
        acc_write_string(*buffer, &offset, change_focus->camera_set);
        acc_write_string(*buffer, &offset, change_focus->camera);
    }

    if (*total != offset) {
        return ACC_STATUS_BUFFER_SIZE_MISMATCH;
    }

    return ACC_STATUS_OK;
}

int32_t acc_write_instant_replay_request(char **buffer, size_t *total, acc_req_instant_replay_t *req_instant_replay) {
    (*total) += sizeof(int8_t);                                   // 1. Request Type
    (*total) += sizeof(int32_t);                                  // 2. Connection ID
    (*total) += sizeof(float);                                    // 3. Start Session Time
    (*total) += sizeof(float);                                    // 4. Duration
    (*total) += sizeof(int32_t);                                  // 5. Car Index Indicator
    (*total) += req_instant_replay->initial_camera_set->size + 2; // 6. Initial Camera Set
    (*total) += req_instant_replay->initial_camera->size + 2;     // 7. Initial Camera

    *buffer = malloc(*total);
    if (*buffer == NULL) {
        return ACC_STATUS_MEMORY_ALLOCATION_ERROR;
    }

    size_t offset = 0;
    acc_write_int8_t(*buffer, &offset, ACC_OUTBOUND_MESSAGE_INSTANT_REPLAY_REQUEST);
    acc_write_int32_t(*buffer, &offset, req_instant_replay->connection_id);
    acc_write_float(*buffer, &offset, req_instant_replay->start_session_time);
    acc_write_float(*buffer, &offset, req_instant_replay->duration);
    acc_write_int32_t(*buffer, &offset, req_instant_replay->initial_focused_car_index);
    acc_write_string(*buffer, &offset, req_instant_replay->initial_camera_set);
    acc_write_string(*buffer, &offset, req_instant_replay->initial_camera);

    if (*total != offset) {
        return ACC_STATUS_BUFFER_SIZE_MISMATCH;
    }

    return ACC_STATUS_OK;
}
