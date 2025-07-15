#include <stdio.h>
#include <stdlib.h>

#include "../include/libracing/acc.h"

typedef struct ACClient {
    int connection_id;
    udp_socket_t *socket;
} acc_client_t;

int acc_write_int8_t(char *buffer, int32_t offset, const int8_t type) {
    buffer[offset++] = type;
    return offset;
}

int acc_write_uint16_t(char *buffer, int32_t offset, const uint16_t data) {
    const size_t size = sizeof(uint16_t);
    char bytes[size];
    memcpy(bytes, &data, size);

    for (int i = 0; i < size; i++) {
        buffer[offset++] = bytes[i];
    }

    return offset;
}

int acc_write_int32_t(char *buffer, int32_t offset, const int32_t data) {
    const size_t size = sizeof(int32_t);
    char bytes[size];
    memcpy(bytes, &data, size);

    for (int i = 0; i < size; i++) {
        buffer[offset++] = bytes[i];
    }

    return offset;
}

int acc_write_float(char *buffer, int offset, const float data) {
    const size_t size = sizeof(int32_t);
    char bytes[size];
    memcpy(bytes, &data, size);

    for (int i = 0; i < size; i++) {
        buffer[offset++] = bytes[i];
    }

    return offset;
}

int acc_write_string(char *buffer, int offset, const string_t *string) {
    for (int i = 0; i < string->size; i++) {
        buffer[offset++] = string->data[i];
        buffer[offset++] = '\0';
    }

    return offset;
}

int32_t acc_write_register_application(char **buffer, const acc_reg_app_t *reg_app) {
    int32_t total = 0;
    int32_t offset = 0;

    total += sizeof(int8_t);                            // 1. Request Type
    total += sizeof(int8_t);                            // 2. Broadcasting Version
    total += reg_app->display_name->size * 2;         // 3. Display Name
    total += reg_app->connection_password->size * 2;  // 4. Connection Password
    total += sizeof(int32_t);                           // 5. Update Interval
    total += reg_app->command_password->size * 2;     // 6. Command Password

    *buffer = malloc(total);

    offset = acc_write_int8_t(*buffer, offset, ACC_OUTBOUND_MESSAGE_REGISTER_COMMAND_APPLICATION);
    offset = acc_write_int8_t(*buffer, offset, ACC_BROADCASTING_PROTOCOL_VERSION);
    offset = acc_write_string(*buffer, offset, reg_app->display_name);
    offset = acc_write_string(*buffer, offset, reg_app->connection_password);
    offset = acc_write_int32_t(*buffer, offset, reg_app->update_interval);
    offset = acc_write_string(*buffer, offset, reg_app->command_password);

    return offset != total ? total : -1;
}

int32_t acc_write_unregister_application(char **buffer, const acc_unreg_app_t *unreg_app) {
    int32_t total = sizeof(int8_t); // 1. Request Type
    int32_t offset = 0;

    *buffer = malloc(total);

    offset = acc_write_int8_t(*buffer, offset, ACC_OUTBOUND_MESSAGE_UNREGISTER_COMMAND_APPLICATION);

    return offset != total ? total : -1;
}

int32_t acc_write_request_entry_list(char **buffer, const acc_req_entry_list_t *req_entry_list) {
    int32_t total = 0;
    int32_t offset = 0;

    total += sizeof(int8_t);    // 1. Request Type
    total += sizeof(int32_t);   // 2. Connection ID

    *buffer = malloc(total);

    offset = acc_write_int8_t(*buffer, offset, ACC_OUTBOUND_MESSAGE_REQUEST_ENTRY_LIST);
    offset = acc_write_int32_t(*buffer, offset, req_entry_list->connection_id);

    return offset != total ? total : -1;
}

int32_t acc_write_request_track_data(char **buffer, const acc_req_track_data_t *req_track_data) {
    int32_t total = 0;
    int32_t offset = 0;

    total += sizeof(int8_t);    // 1. Request Type
    total += sizeof(int32_t);   // 2. Connection ID

    *buffer = malloc(total);

    offset = acc_write_int8_t(*buffer, offset, ACC_OUTBOUND_MESSAGE_REQUEST_TRACK_DATA);
    offset = acc_write_int32_t(*buffer, offset, req_track_data->connection_id);

    return offset != total ? total : -1;
}

int32_t acc_write_change_hud_page(char **buffer, const acc_change_hud_page_t *change_hud_page) {
    int32_t total = 0;
    int32_t offset = 0;

    total += sizeof(int8_t);    // 1. Request Type
    total += sizeof(int32_t);   // 2. Connection ID
    total += change_hud_page->hud_page->size * 2; // 3. HUD Page

    *buffer = malloc(total);

    offset = acc_write_int8_t(*buffer, offset, ACC_OUTBOUND_MESSAGE_CHANGE_HUD_PAGE);
    offset = acc_write_int32_t(*buffer, offset, change_hud_page->connection_id);
    offset = acc_write_string(*buffer, offset, change_hud_page->hud_page);

    return offset != total ? total : -1;
}

int32_t acc_write_change_focus(char **buffer, const acc_change_focus_t *change_focus) {
    int32_t total = 0;
    int32_t offset = 0;

    total += sizeof(int8_t);                                                                // 1. Request Type
    total += sizeof(int32_t);                                                               // 2. Connection ID
    total += sizeof(int8_t);                                                                // 3.1. Car Index Indicator
    if (change_focus->car_index != NULL) total += sizeof(uint16_t);                         // 3.2. Car Index
    total += sizeof(int8_t);                                                                // 4.1. Camera Indicator
    if (change_focus->camera_set != NULL) total += change_focus->camera_set->size * 2;    // 4.2. Camera Set
    if (change_focus->camera != NULL) total += change_focus->camera->size * 2;            // 4.2. Camera

    *buffer = malloc(total);

    offset = acc_write_int8_t(*buffer, offset, ACC_OUTBOUND_MESSAGE_CHANGE_FOCUS);
    offset = acc_write_int32_t(*buffer, offset, change_focus->connection_id);
    if (change_focus->car_index != NULL) {
        offset = acc_write_int8_t(*buffer, offset, 1);
        offset = acc_write_uint16_t(*buffer, offset, *change_focus->car_index);
    } else {
        offset = acc_write_int8_t(*buffer, offset, 0);
    }
    if (change_focus->camera_set != NULL || change_focus->camera != NULL) {
        offset = acc_write_int8_t(*buffer, offset, 1);
        if (change_focus->camera_set != 0) {
            offset = acc_write_string(*buffer, offset, change_focus->camera_set);
        }
        if (change_focus->camera != 0) {
            offset = acc_write_string(*buffer, offset, change_focus->camera);
        }
    }

    return offset != total ? total : -1;
}

int32_t acc_write_instant_replay_request(char **buffer, const acc_req_instant_replay_t *req_instant_replay) {
    int32_t total = 0;
    int32_t offset = 0;

    total += sizeof(int8_t);                                        // 1. Request Type
    total += sizeof(int32_t);                                       // 2. Connection ID
    total += sizeof(float);                                         // 3. Start Session Time
    total += sizeof(float);                                         // 4. Duration
    total += sizeof(int32_t);                                       // 5. Car Index Indicator
    total += req_instant_replay->initial_camera_set->size * 2;    // 6. Initial Camera Set
    total += req_instant_replay->initial_camera->size * 2;        // 7. Initial Camera

    *buffer = malloc(total);

    offset = acc_write_int8_t(*buffer, offset, ACC_OUTBOUND_MESSAGE_INSTANT_REPLAY_REQUEST);
    offset = acc_write_int32_t(*buffer, offset, req_instant_replay->connection_id);
    offset = acc_write_float(*buffer, offset, req_instant_replay->start_session_time);
    offset = acc_write_float(*buffer, offset, req_instant_replay->duration);
    offset = acc_write_int32_t(*buffer, offset, req_instant_replay->initial_focused_car_index);
    offset = acc_write_string(*buffer, offset, req_instant_replay->initial_camera_set);
    offset = acc_write_string(*buffer, offset, req_instant_replay->initial_camera);

    return offset != total ? total : -1;
}

int32_t acc_read_string(string_t *string, const char *buffer, int32_t offset) {
    const uint16_t size = (uint16_t)buffer + offset;
    offset += sizeof(uint16_t);

    string->size = size;
    string->data = malloc(size);
    memcpy(string->data, buffer + offset, size);

    return offset + size;
}

int32_t acc_read_bool(bool *data, const char *buffer, const int32_t offset) {
    memcpy(data, buffer + offset, sizeof(bool));
    return offset + (int32_t)sizeof(bool);
}

int32_t acc_read_int8_t(int8_t *data, const char *buffer, const int32_t offset) {
    memcpy(data, buffer + offset, sizeof(int8_t));
    return offset + (int32_t)sizeof(int8_t);
}

int32_t acc_read_int16_t(int16_t *data, const char *buffer, const int32_t offset) {
    memcpy(data, buffer + offset, sizeof(int16_t));
    return offset + (int32_t)sizeof(int16_t);
}

int32_t acc_read_int32_t(int32_t *data, const char *buffer, const int32_t offset) {
    memcpy(data, buffer + offset, sizeof(int32_t));
    return offset + (int32_t)sizeof(int32_t);
}

int32_t acc_read_uint16_t(uint16_t *data, const char *buffer, const int32_t offset) {
    memcpy(data, buffer + offset, sizeof(uint16_t));
    return offset + (int32_t)sizeof(uint16_t);
}

int32_t acc_read_float(float *data, const char *buffer, const int32_t offset) {
    memcpy(data, buffer + offset, sizeof(float));
    return offset + (int32_t)sizeof(float);
}

int32_t acc_read_lap(acc_lap_info_t *data, const char *buffer, int32_t offset) {
    offset = acc_read_int32_t(&data->lap_time, buffer, offset); // 1. Lap Time
    offset = acc_read_uint16_t(&data->car_index, buffer, offset); // 2. Car Index
    offset = acc_read_uint16_t(&data->driver_index, buffer, offset); // 3. Driver Index

    int8_t splits_size = 0;
    offset = acc_read_int8_t(&splits_size, buffer, offset); // 4. Splits Size
    data->splits = malloc(sizeof(int32_array_t));
    data->splits->size = (uint32_t)splits_size;
    data->splits->data = malloc(sizeof(int32_t) * splits_size);

    for (int index = 0; index < splits_size; index++) {
        offset = acc_read_int32_t(&data->splits->data[index], buffer, offset); // 5. Split Time
    }

    offset = acc_read_bool(&data->is_invalid, buffer, offset); // 6. Is Invalid Flag
    offset = acc_read_bool(&data->is_valid_for_best, buffer, offset); // 7. Is Valid For Best Flag

    bool is_outlap = false;
    bool is_inlap = false;

    offset = acc_read_bool(&is_outlap, buffer, offset); // 8. Is Outlap Flag
    offset = acc_read_bool(&is_inlap, buffer, offset); // 9. Is Inlap Flag

    if (is_outlap == true) {
        data->lap_type = ACC_LAP_TYPE_OUTLAP;
    } else if (is_inlap == true) {
        data->lap_type = ACC_LAP_TYPE_INLAP;
    } else {
        data->lap_type = ACC_LAP_TYPE_REGULAR;
    }

    if (data->splits->size < 3) {
        const uint32_t old = data->splits->size;
        data->splits->size = 3;
        data->splits->data = realloc(data->splits->data, sizeof(int32_t) * data->splits->size);

        for (uint32_t index = old; index < data->splits->size; index++) {
            data->splits->data[index] = INT32_MAX;
        }
    }

    return offset;
}

int32_t acc_read_registration_result(acc_reg_result_t *result, const char *buffer) {
    int32_t offset = 1;

    result->error_message = malloc(sizeof(string_t));

    offset = acc_read_int32_t(&result->connection_id, buffer, offset); // 1. Connection ID
    offset = acc_read_bool(&result->connection_success, buffer, offset); // 2. Connection Success
    offset = acc_read_bool(&result->is_read_only, buffer, offset); // 3. Is Read-Only
    offset = acc_read_string(result->error_message, buffer, offset); // 4. Error Message

    return offset;
}

int32_t acc_read_real_time_update(acc_rt_update_t *update, const char *buffer) {
    int32_t offset = 1;

    update->active_camera_set = malloc(sizeof(string_t));
    update->active_camera = malloc(sizeof(string_t));
    update->current_hud_page = malloc(sizeof(string_t));

    offset = acc_read_uint16_t(&update->event_index, buffer, offset); // 1. Event Index
    offset = acc_read_uint16_t(&update->session_index, buffer, offset); // 2. Session Index
    offset = acc_read_int8_t(&update->session_type, buffer, offset); // 3. Session Type
    offset = acc_read_int8_t(&update->phase, buffer, offset); // 4. Phase
    offset = acc_read_float(&update->session_time, buffer, offset); // 5. Session Type
    offset = acc_read_float(&update->session_end_time, buffer, offset); // 6. Session Type
    offset = acc_read_int32_t(&update->focused_car_index, buffer, offset); // 7. Track Length
    offset = acc_read_string(update->active_camera_set, buffer, offset); // 8. Track Temperature
    offset = acc_read_string(update->active_camera, buffer, offset); // 9. Track Rain
    offset = acc_read_string(update->current_hud_page, buffer, offset); // 10. Track Wind Direction

    offset = acc_read_bool(&update->is_replay_playing, buffer, offset); // 11. Is Replay Playing Flag
    if (update->is_replay_playing == true) {
        offset = acc_read_float(&update->replay_session_time, buffer, offset); // 12. Replay Session Time
        offset = acc_read_float(&update->replay_remaining_time, buffer, offset); // 13. Replay Remaining Time
    }

    offset = acc_read_float(&update->time_of_day, buffer, offset); // 14. Time of Day
    offset = acc_read_int8_t(&update->ambient_temperature, buffer, offset); // 15. Ambient Temperature
    offset = acc_read_int8_t(&update->track_temperature, buffer, offset); // 16. Track Temperature
    offset = acc_read_int8_t(&update->cloud_level, buffer, offset); // 17. Cloud Level
    offset = acc_read_int8_t(&update->rain_level, buffer, offset); // 18. Rain Level
    offset = acc_read_int8_t(&update->wetness_level, buffer, offset); // 19. Wetness Level
    offset = acc_read_lap(&update->best_session_lap, buffer, offset); // 20. Best Session Lap

    return offset;
}

int32_t acc_read_real_time_car_update(acc_rt_car_update_t *update, const char *buffer) {
    int32_t offset = 1;

    offset = acc_read_uint16_t(&update->car_index, buffer, offset); // 1. Car Index
    offset = acc_read_uint16_t(&update->driver_index, buffer, offset); // 2. Driver Index
    offset = acc_read_int8_t(&update->gear, buffer, offset); // 3. Gear
    offset = acc_read_float(&update->world_position_x, buffer, offset); // 4. World Position X
    offset = acc_read_float(&update->world_position_y, buffer, offset); // 5. World Position Y
    offset = acc_read_float(&update->yaw, buffer, offset); // 6. Yaw
    offset = acc_read_int8_t(&update->car_location, buffer, offset); // 7. Car Location
    offset = acc_read_uint16_t(&update->speed, buffer, offset); // 8. Speed
    offset = acc_read_uint16_t(&update->position, buffer, offset); // 9. Position
    offset = acc_read_uint16_t(&update->cup_position, buffer, offset); // 10. Cup Position
    offset = acc_read_uint16_t(&update->track_position, buffer, offset); // 11. Track Position
    offset = acc_read_float(&update->spline_position, buffer, offset); // 12. Spline Position
    offset = acc_read_uint16_t(&update->laps, buffer, offset); // 13. Laps
    offset = acc_read_int32_t(&update->delta, buffer, offset); // 14. Delta
    offset = acc_read_lap(&update->best_session_lap, buffer, offset); // 15. Best Session Lap
    offset = acc_read_lap(&update->last_lap, buffer, offset); // 16. Last Lap
    offset = acc_read_lap(&update->current_lap, buffer, offset); // 17. Current Lap

    return offset;
}

int32_t acc_read_entry_list(acc_entry_list_t *list, const char *buffer) {
    int32_t offset = 1;
    uint16_t size = 0;

    offset = acc_read_int32_t(&list->connection_id, buffer, offset); // 1. Connection ID
    offset = acc_read_uint16_t(&size, buffer, offset); // 2. Entries Size

    list->indexes = malloc(sizeof(uint16_array_t));
    list->indexes->size = (uint32_t)size;
    list->indexes->data = malloc(sizeof(uint16_t) * size);

    for (int i = 0; i < size; i++) {
        offset = acc_read_uint16_t(&list->indexes->data[i], buffer, offset);
    }

    return offset;
}

int32_t acc_read_entry_list_car(acc_car_info_t *data, const char *buffer) {
    int32_t offset = 1;
    int8_t size = 0;

    offset = acc_read_uint16_t(&data->car_index, buffer, offset); // 1. Connection ID
    offset = acc_read_int8_t(&data->car_model_type, buffer, offset); // 2. Car Model Type
    offset = acc_read_string(data->team_name, buffer, offset); // 3. Team Name
    offset = acc_read_int32_t(&data->race_number, buffer, offset); // 4. Race Number
    offset = acc_read_int8_t(&data->cup_category, buffer, offset); // 5. Cup Category
    offset = acc_read_int8_t(&data->current_driver_index, buffer, offset); // 6. Current Driver Index
    offset = acc_read_int8_t(&size, buffer, offset); // 7. Drivers Size

    data->drivers_info = malloc(sizeof(acc_driver_info_array_t));
    data->drivers_info->size = (uint32_t)size;
    data->drivers_info->data = malloc(sizeof(acc_driver_info_t) * size);

    for (int i = 0; i < size; i++) {
        offset = acc_read_string(data->drivers_info->data[i].first_name, buffer, offset); // 8. First Name
        offset = acc_read_string(data->drivers_info->data[i].last_name, buffer, offset); // 9. Last Name
        offset = acc_read_string(data->drivers_info->data[i].short_name, buffer, offset); // 10. Short Name
        offset = acc_read_int8_t(&data->drivers_info->data[i].category, buffer, offset); // 11. Category
        offset = acc_read_uint16_t(&data->drivers_info->data[i].nationality, buffer, offset); // 12. Nationality
    }

    return offset;
}

acc_client_t *acc_client_create(const char *address, const int port) {
    acc_client_t *client = malloc(sizeof(acc_client_t));
    if (client == NULL) {
        acc_client_destroy(client);
        return NULL;
    }

    client->socket = udp_socket_create(address, port);
    if (client->socket == NULL) {
        acc_client_destroy(client);
        return NULL;
    }

    return client;
}

void acc_client_close(acc_client_t *client) {
    if (client == NULL) return;
    if (client->socket != NULL) udp_socket_destroy(client->socket);
    free(client);
}

acc_status_t acc_client_send(acc_client_t *client, acc_server_request_t *request) {
    if (client == NULL) return ACC_STATUS_CLIENT_NOT_INITIALIZED;
    if (client->socket == NULL) return ACC_STATUS_CLIENT_NOT_INITIALIZED;

    char *buffer = NULL;
    int32_t total = 0;

    switch (request->type) {
        case ACC_OUTBOUND_MESSAGE_REGISTER_COMMAND_APPLICATION:
            total = acc_write_register_application(&buffer, request->data.register_application);
            break;
        case ACC_OUTBOUND_MESSAGE_UNREGISTER_COMMAND_APPLICATION:
            total = acc_write_unregister_application(&buffer, request->data.unregister_application);
            break;
        case ACC_OUTBOUND_MESSAGE_REQUEST_ENTRY_LIST:
            total = acc_write_request_entry_list(&buffer, request->data.request_entry_list);
            break;
        case ACC_OUTBOUND_MESSAGE_REQUEST_TRACK_DATA:
            total = acc_write_request_track_data(&buffer, request->data.request_track_data);
            break;
        case ACC_OUTBOUND_MESSAGE_CHANGE_HUD_PAGE:
            total = acc_write_change_hud_page(&buffer, request->data.request_hud_page);
            break;
        case ACC_OUTBOUND_MESSAGE_CHANGE_FOCUS:
            total = acc_write_change_focus(&buffer, request->data.change_focus);
            break;
        case ACC_OUTBOUND_MESSAGE_INSTANT_REPLAY_REQUEST:
            total = acc_write_instant_replay_request(&buffer, request->data.request_instant_replay);
            break;
        default:
            fprintf(stderr, "ACC: Invalid Outbound Message Type: %d\n", request->type);
            return ACC_STATUS_INVALID_OUTBOUND_MESSAGE_TYPE;
    }

    if (total <= 0) {
        fprintf(stderr, "ACC: acc_write_outbound_message_type failed\n");
        free(buffer);
        return ACC_STATUS_BUFFER_SIZE_MISMATCH;
    }

    const int sent = udp_socket_send(client->socket, buffer, total);
    if (sent < 0) {
        fprintf(stderr, "ACC: udp_socket_send failed\n");
        free(buffer);
        return ACC_STATUS_SOCKET_ERROR;
    }

    if (sent != total) {
        fprintf(stderr, "ACC: Unexpected amount of bytes send: Expected: %d; Actual: %d\n", total, sent);
        free(buffer);
        return ACC_STATUS_UNEXPECTED_AMOUNT_OF_BYTES_SENT;
    }

    free(buffer);
    return sent == total ? ACC_STATUS_OK : ACC_STATUS_SEND_ERROR;
}

acc_status_t acc_client_receive(acc_client_t *client, acc_server_response_t *response) {
    if (client == NULL) return ACC_STATUS_CLIENT_NOT_INITIALIZED;
    if (client->socket == NULL) return ACC_STATUS_CLIENT_NOT_INITIALIZED;

    const int size = 1024 * 1024 * 10;
    char *buffer = malloc(size);
    if (buffer == NULL) {
        fprintf(stderr, "ACC: malloc failed\n");
        return ACC_STATUS_MEMORY_ALLOCATION_ERROR;
    }

    const int received = udp_socket_receive(client->socket, buffer, size);
    if (received < 0) {
        free(buffer);
        fprintf(stderr, "ACC: udp_socket_receive failed\n");
        return ACC_STATUS_SOCKET_ERROR;
    }

    const int8_t response_type = buffer[0];
    int32_t offset = 1;

    switch (response_type) {
        case ACC_INBOUND_MESSAGE_REGISTRATION_RESULT:
            response->type = ACC_INBOUND_MESSAGE_REGISTRATION_RESULT;
            response->data.registration_result = malloc(sizeof(acc_reg_result_t));
            offset = acc_read_registration_result(response->data.registration_result, buffer);
            break;
        case ACC_INBOUND_MESSAGE_REAL_TIME_UPDATE:
            response->type = ACC_INBOUND_MESSAGE_REAL_TIME_UPDATE;
            response->data.real_time_update = malloc(sizeof(acc_rt_update_t));
            offset = acc_read_real_time_update(response->data.real_time_update, buffer);
            break;
        case ACC_INBOUND_MESSAGE_REAL_TIME_CAR_UPDATE:
            response->type = ACC_INBOUND_MESSAGE_REAL_TIME_CAR_UPDATE;
            response->data.real_time_car_update = malloc(sizeof(acc_rt_car_update_t));
            offset = acc_read_real_time_car_update(response->data.real_time_car_update, buffer);
            break;
        case ACC_INBOUND_MESSAGE_ENTRY_LIST:
            response->type = ACC_INBOUND_MESSAGE_ENTRY_LIST;
            response->data.entry_list = malloc(sizeof(acc_entry_list_t));
            offset = acc_read_entry_list(response->data.entry_list, buffer);
            break;
        case ACC_INBOUND_MESSAGE_ENTRY_LIST_CAR:
            response->type = ACC_INBOUND_MESSAGE_ENTRY_LIST_CAR;
            response->data.entry_list_car = malloc(sizeof(acc_car_info_t));
            break;
        case ACC_INBOUND_MESSAGE_TRACK_DATA:
            response->type = ACC_INBOUND_MESSAGE_TRACK_DATA;
            response->data.track_data = malloc(sizeof(acc_track_data_t));
            break;
        case ACC_INBOUND_MESSAGE_BROADCASTING_EVENT:
            response->type = ACC_INBOUND_MESSAGE_BROADCASTING_EVENT;
            response->data.broadcasting_event = malloc(sizeof(acc_broadcasting_event_t));
            break;
        default:
            fprintf(stderr, "ACC: Invalid Inbound Message Type: %d\n", response_type);
            free(buffer);
            return ACC_STATUS_INVALID_INBOUND_MESSAGE_TYPE;
    }

    if (offset > size) {
        fprintf(stderr, "ACC: Inbound Buffer too small: %d\n", response_type);
        free(buffer);
        return ACC_STATUS_INBOUND_BUFFER_TOO_SMALL;
    }

    free(buffer);
    return ACC_STATUS_OK;
}
