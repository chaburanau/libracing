#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/libracing/acc.h"
#include "../../include/libracing/types.h"

void acc_read_int16_t(int16_t *data, char *buffer, size_t *offset) {
    memcpy(data, buffer + *offset, sizeof(int16_t));
    (*offset) += sizeof(int16_t);
}

void acc_read_int32_t(int32_t *data, char *buffer, size_t *offset) {
    memcpy(data, buffer + *offset, sizeof(int32_t));
    (*offset) += sizeof(int32_t);
}

void acc_read_uint8_t(uint8_t *data, char *buffer, size_t *offset) {
    memcpy(data, buffer + *offset, sizeof(uint8_t));
    (*offset) += sizeof(uint8_t);
}

void acc_read_uint16_t(uint16_t *data, char *buffer, size_t *offset) {
    memcpy(data, buffer + *offset, sizeof(uint16_t));
    (*offset) += sizeof(uint16_t);
}

void acc_read_float(float *data, char *buffer, size_t *offset) {
    memcpy(data, buffer + *offset, sizeof(float));
    (*offset) += sizeof(float);
}

void acc_read_bool(bool *data, char *buffer, size_t *offset) {
    uint8_t value;
    acc_read_uint8_t(&value, buffer, offset);

    if (value > 0) {
        (*data) = true;
    } else {
        (*data) = false;
    }
}

void acc_read_string(string_t *string, char *buffer, size_t *offset) {
    uint16_t size;
    acc_read_uint16_t(&size, buffer, offset);

    string->size = size;
    string->data = malloc((size_t)size);
    memcpy(string->data, buffer + *offset, size);

    (*offset) += size;
}

void acc_read_lap(acc_lap_info_t *data, char *buffer, size_t *offset) {
    uint8_t splits_size = 0;

    acc_read_int32_t(&data->lap_time, buffer, offset);      // 1. Lap Time
    acc_read_uint16_t(&data->car_index, buffer, offset);    // 2. Car Index
    acc_read_uint16_t(&data->driver_index, buffer, offset); // 3. Driver Index
    acc_read_uint8_t(&splits_size, buffer, offset);         // 4. Splits Size

    data->splits = malloc(sizeof(int32_array_t));
    data->splits->size = (uint32_t)splits_size;
    data->splits->data = malloc(sizeof(int32_t) * (size_t)splits_size);

    for (uint8_t index = 0; index < splits_size; index++) {
        acc_read_int32_t(&data->splits->data[index], buffer, offset); // 5. Split Time
    }

    acc_read_bool(&data->is_invalid, buffer, offset);        // 6. Is Invalid Flag
    acc_read_bool(&data->is_valid_for_best, buffer, offset); // 7. Is Valid For Best Flag

    bool is_outlap = false;
    bool is_inlap = false;

    acc_read_bool(&is_outlap, buffer, offset); // 8. Is Outlap Flag
    acc_read_bool(&is_inlap, buffer, offset);  // 9. Is Inlap Flag

    if (is_outlap == true) {
        data->lap_type = ACC_LAP_TYPE_OUTLAP;
    } else if (is_inlap == true) {
        data->lap_type = ACC_LAP_TYPE_INLAP;
    } else {
        data->lap_type = ACC_LAP_TYPE_REGULAR;
    }

    if (data->splits->size < 3) {
        uint32_t old = data->splits->size;
        data->splits->size = 3;
        data->splits->data = realloc(data->splits->data, sizeof(int32_t) * data->splits->size);

        for (uint8_t index = (uint8_t)old; index < data->splits->size; index++) {
            data->splits->data[index] = INT32_MAX;
        }
    }
}

void acc_read_registration_result(acc_reg_result_t *result, char *buffer, size_t *offset) {
    result->error_message = malloc(sizeof(string_t));

    acc_read_int32_t(&result->connection_id, buffer, offset);   // 1. Connection ID
    acc_read_bool(&result->connection_success, buffer, offset); // 2. Connection Success
    acc_read_bool(&result->is_read_only, buffer, offset);       // 3. Is Read-Only
    acc_read_string(result->error_message, buffer, offset);     // 4. Error Message
}

void acc_read_real_time_update(acc_rt_update_t *update, char *buffer, size_t *offset) {
    update->active_camera_set = malloc(sizeof(string_t));
    update->active_camera = malloc(sizeof(string_t));
    update->current_hud_page = malloc(sizeof(string_t));

    acc_read_uint16_t(&update->event_index, buffer, offset);      // 1. Event Index
    acc_read_uint16_t(&update->session_index, buffer, offset);    // 2. Session Index
    acc_read_uint8_t(&update->session_type, buffer, offset);      // 3. Session Type
    acc_read_uint8_t(&update->phase, buffer, offset);             // 4. Phase
    acc_read_float(&update->session_time, buffer, offset);        // 5. Session Type
    acc_read_float(&update->session_end_time, buffer, offset);    // 6. Session Type
    acc_read_int32_t(&update->focused_car_index, buffer, offset); // 7. Track Length
    acc_read_string(update->active_camera_set, buffer, offset);   // 8. Track Temperature
    acc_read_string(update->active_camera, buffer, offset);       // 9. Track Rain
    acc_read_string(update->current_hud_page, buffer, offset);    // 10. Track Wind Direction

    acc_read_bool(&update->is_replay_playing, buffer, offset); // 11. Is Replay Playing Flag
    if (update->is_replay_playing == true) {
        acc_read_float(&update->replay_session_time, buffer, offset);   // 12. Replay Session Time
        acc_read_float(&update->replay_remaining_time, buffer, offset); // 13. Replay Remaining Time
    }

    acc_read_float(&update->time_of_day, buffer, offset);           // 14. Time of Day
    acc_read_uint8_t(&update->ambient_temperature, buffer, offset); // 15. Ambient Temperature
    acc_read_uint8_t(&update->track_temperature, buffer, offset);   // 16. Track Temperature
    acc_read_uint8_t(&update->cloud_level, buffer, offset);         // 17. Cloud Level
    acc_read_uint8_t(&update->rain_level, buffer, offset);          // 18. Rain Level
    acc_read_uint8_t(&update->wetness_level, buffer, offset);       // 19. Wetness Level
    acc_read_lap(&update->best_session_lap, buffer, offset);        // 20. Best Session Lap
}

void acc_read_real_time_car_update(acc_rt_car_update_t *update, char *buffer, size_t *offset) {
    acc_read_uint16_t(&update->car_index, buffer, offset);      // 1. Car Index
    acc_read_uint16_t(&update->driver_index, buffer, offset);   // 2. Driver Index
    acc_read_uint8_t(&update->driver_count, buffer, offset);    // 3. Driver Count
    acc_read_uint8_t(&update->gear, buffer, offset);            // 4. Gear
    acc_read_float(&update->world_position_x, buffer, offset);  // 5. World Position X
    acc_read_float(&update->world_position_y, buffer, offset);  // 6. World Position Y
    acc_read_float(&update->yaw, buffer, offset);               // 7. Yaw
    acc_read_uint8_t(&update->car_location, buffer, offset);    // 7. Car Location
    acc_read_uint16_t(&update->speed, buffer, offset);          // 8. Speed
    acc_read_uint16_t(&update->position, buffer, offset);       // 9. Position
    acc_read_uint16_t(&update->cup_position, buffer, offset);   // 10. Cup Position
    acc_read_uint16_t(&update->track_position, buffer, offset); // 11. Track Position
    acc_read_float(&update->spline_position, buffer, offset);   // 12. Spline Position
    acc_read_uint16_t(&update->laps, buffer, offset);           // 13. Laps
    acc_read_int32_t(&update->delta, buffer, offset);           // 14. Delta
    acc_read_lap(&update->best_session_lap, buffer, offset);    // 15. Best Session Lap
    acc_read_lap(&update->last_lap, buffer, offset);            // 16. Last Lap
    acc_read_lap(&update->current_lap, buffer, offset);         // 17. Current Lap
}

void acc_read_entry_list(acc_entry_list_t *list, char *buffer, size_t *offset) {
    uint16_t size = 0;

    acc_read_int32_t(&list->connection_id, buffer, offset); // 1. Connection ID
    acc_read_uint16_t(&size, buffer, offset);               // 2. Entries Size

    list->indexes = malloc(sizeof(uint16_array_t));
    list->indexes->size = (uint32_t)size;
    list->indexes->data = malloc(sizeof(uint16_t) * (size_t)size);

    for (uint8_t i = 0; i < size; i++) {
        acc_read_uint16_t(&list->indexes->data[i], buffer, offset);
    }
}

void acc_read_entry_list_car(acc_car_info_t *data, char *buffer, size_t *offset) {
    uint8_t size = 0;

    acc_read_uint16_t(&data->car_index, buffer, offset);           // 1. Connection ID
    acc_read_uint8_t(&data->car_model_type, buffer, offset);       // 2. Car Model Type
    acc_read_string(data->team_name, buffer, offset);              // 3. Team Name
    acc_read_int32_t(&data->race_number, buffer, offset);          // 4. Race Number
    acc_read_uint8_t(&data->cup_category, buffer, offset);         // 5. Cup Category
    acc_read_uint8_t(&data->current_driver_index, buffer, offset); // 6. Current Driver Index
    acc_read_uint16_t(&data->nationality, buffer, offset);         // 7. Nationality

    acc_read_uint8_t(&size, buffer, offset); // 8. Drivers Size

    data->drivers_info = malloc(sizeof(acc_driver_info_array_t));
    data->drivers_info->size = (uint32_t)size;
    data->drivers_info->data = malloc(sizeof(acc_driver_info_t) * (size_t)size);

    for (uint8_t i = 0; i < size; i++) {
        acc_read_string(data->drivers_info->data[i].first_name, buffer, offset);     // 9. First Name
        acc_read_string(data->drivers_info->data[i].last_name, buffer, offset);      // 10. Last Name
        acc_read_string(data->drivers_info->data[i].short_name, buffer, offset);     // 11. Short Name
        acc_read_uint8_t(&data->drivers_info->data[i].category, buffer, offset);     // 12. Category
        acc_read_uint16_t(&data->drivers_info->data[i].nationality, buffer, offset); // 13. Nationality
    }
}

// TODO :: hashmap approach
void acc_read_track_data(acc_track_data_t *data, char *buffer, size_t *offset) {
    uint8_t camera_set_size = 0;
    uint8_t hud_page_size = 0;

    data->track_name = malloc(sizeof(string_t));
    data->camera_sets = malloc(sizeof(string_array_t));
    data->cameras = malloc(sizeof(string_array_t));
    data->hud_pages = malloc(sizeof(string_array_t));

    acc_read_int32_t(&data->connection_id, buffer, offset); // 1. Connection ID
    acc_read_string(data->track_name, buffer, offset);      // 2. Track Name
    acc_read_int32_t(&data->track_id, buffer, offset);      // 3. Track ID
    acc_read_int32_t(&data->track_length, buffer, offset);  // 4. Track Length
    acc_read_uint8_t(&camera_set_size, buffer, offset);     // 5. Camera Set Size

    data->camera_sets->size = (uint32_t)camera_set_size;
    data->camera_sets->data = malloc(sizeof(string_t) * (size_t)camera_set_size);

    for (uint8_t camera_set_index = 0; camera_set_index < camera_set_size; camera_set_index++) {
        uint8_t camera_size = 0;
        uint32_t previous_camera_size = data->cameras->size;

        acc_read_string(data->camera_sets->data[camera_set_index], buffer, offset); // 6. Camera Set Item
        acc_read_uint8_t(&camera_size, buffer, offset);                             // 7. Camera Size

        data->cameras->size += (uint32_t)camera_size;
        data->cameras->data = realloc(data->cameras->data, sizeof(string_t) * data->cameras->size);

        for (uint8_t camera_index = 0; camera_index < camera_size; camera_index++) {
            acc_read_string(data->cameras->data[previous_camera_size + camera_index], buffer, offset); // 8. Camera Item
        }
    }

    acc_read_uint8_t(&hud_page_size, buffer, offset); // 9. HUD Page Size

    data->hud_pages->size = (uint32_t)hud_page_size;
    data->hud_pages->data = malloc(sizeof(string_t) * (size_t)hud_page_size);

    for (int index = 0; index < camera_set_size; index++) {
        acc_read_string(data->hud_pages->data[index], buffer, offset); // 10. HUD Page Item
    }
}

void acc_read_broadcasting_event(acc_broadcasting_event_t *data, char *buffer, size_t *offset) {
    data->message = malloc(sizeof(string_t));

    acc_read_uint8_t(&data->type, buffer, offset);      // 1. Type
    acc_read_string(data->message, buffer, offset);     // 2. Message
    acc_read_int32_t(&data->time, buffer, offset);      // 3. Time
    acc_read_int32_t(&data->car_index, buffer, offset); // 4. Car Index
}
