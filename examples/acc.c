#include "../src/acc.c"
#include <stdint.h>
#include <stdlib.h>

static int32_t connection_id;

void check_error(bool has_error, char *operation) {
    if (has_error == true) {
        fprintf(stderr, "%s(...) failed with error %d\n", operation, acc_get_last_error());
        exit(-1);
    }
}

string_t *new_string(char *data, size_t size) {
    string_t *result = malloc(sizeof(string_t));
    result->size = (uint32_t)size;
    result->data = data;
    return result;
}

void receive_response(acc_client_t *client) {
    acc_server_response_t response;

    bool has_error = acc_client_receive(client, &response);
    check_error(has_error, "acc_client_receive");

    fprintf(stdout, "Response has been received...\n");
    fprintf(stdout, "\tResponse Type: %d\n", response.type);

    switch (response.type) {
    case ACC_INBOUND_MESSAGE_REGISTRATION_RESULT:
        fprintf(stdout, "\tConnection ID: %d\n", response.data.registration_result->connection_id);
        fprintf(stdout, "\tConnection Success: %d\n", response.data.registration_result->connection_success);
        fprintf(stdout, "\tIs Read Only: %d\n", response.data.registration_result->is_read_only);
        fprintf(stdout, "\tError Message Size: %d\n", response.data.registration_result->error_message->size);
        fprintf(stdout, "\tError Message Data: %s\n", response.data.registration_result->error_message->data);

        connection_id = response.data.registration_result->connection_id;
        return;
    case ACC_INBOUND_MESSAGE_REAL_TIME_UPDATE:
        fprintf(stdout, "\tEvent Index: %d\n", response.data.real_time_update->event_index);
        fprintf(stdout, "\tSession Index: %d\n", response.data.real_time_update->session_index);
        fprintf(stdout, "\tPhase: %d\n", response.data.real_time_update->phase);
        fprintf(stdout, "\tSession Time: %f\n", response.data.real_time_update->session_time);
        fprintf(stdout, "\tRemaining Time: %f\n", response.data.real_time_update->remaining_time);
        fprintf(stdout, "\tSession Type: %d\n", response.data.real_time_update->session_type);
        fprintf(stdout, "\tAmbient Temperature: %d\n", response.data.real_time_update->ambient_temperature);
        fprintf(stdout, "\tTrack Temperature: %d\n", response.data.real_time_update->track_temperature);
        return;
    case ACC_INBOUND_MESSAGE_REAL_TIME_CAR_UPDATE:
        fprintf(stdout, "\tCar Index: %d\n", response.data.real_time_car_update->car_index);
        fprintf(stdout, "\tDriver Index: %d\n", response.data.real_time_car_update->driver_index);
        fprintf(stdout, "\tGear: %d\n", response.data.real_time_car_update->gear);
        fprintf(stdout, "\tSpeed: %d\n", response.data.real_time_car_update->speed);
        fprintf(stdout, "\tPosition: %d\n", response.data.real_time_car_update->position);
        fprintf(stdout, "\tLaps: %d\n", response.data.real_time_car_update->laps);
        fprintf(stdout, "\tDrivers Count: %d\n", response.data.real_time_car_update->driver_count);
        return;
    case ACC_INBOUND_MESSAGE_ENTRY_LIST:
        fprintf(stdout, "\tConnection ID: %d\n", response.data.entry_list->connection_id);
        fprintf(stdout, "\tIndexes Size: %d\n", response.data.entry_list->indexes->size);
        return;
    case ACC_INBOUND_MESSAGE_ENTRY_LIST_CAR:
        fprintf(stdout, "\tCar Index: %d\n", response.data.entry_list_car->car_index);
        fprintf(stdout, "\tCar Model Type: %d\n", response.data.entry_list_car->car_model_type);
        fprintf(stdout, "\tTeam Name Size: %d\n", response.data.entry_list_car->team_name->size);
        fprintf(stdout, "\tTeam Name Data: %s\n", response.data.entry_list_car->team_name->data);
        fprintf(stdout, "\tNationality: %d\n", response.data.entry_list_car->nationality);
        fprintf(stdout, "\tRace Number: %d\n", response.data.entry_list_car->race_number);
        fprintf(stdout, "\tDrivers Info Size: %d\n", response.data.entry_list_car->drivers_info->size);
        return;
    case ACC_INBOUND_MESSAGE_TRACK_DATA:
        fprintf(stdout, "\tConnection ID: %d\n", response.data.track_data->connection_id);
        fprintf(stdout, "\tTrack ID: %d\n", response.data.track_data->track_id);
        fprintf(stdout, "\tTrack Name Size: %d\n", response.data.track_data->track_name->size);
        fprintf(stdout, "\tTrack Name Data: %s\n", response.data.track_data->track_name->data);
        fprintf(stdout, "\tTrack Length: %d\n", response.data.track_data->track_length);
        return;
    case ACC_INBOUND_MESSAGE_BROADCASTING_EVENT:
        fprintf(stdout, "\tCar Index: %d\n", response.data.broadcasting_event->car_index);
        fprintf(stdout, "\tType: %d\n", response.data.broadcasting_event->type);
        fprintf(stdout, "\tTime: %d\n", response.data.broadcasting_event->time);
        fprintf(stdout, "\tMessage Size: %d\n", response.data.broadcasting_event->message->size);
        fprintf(stdout, "\tMessage Data: %s\n", response.data.broadcasting_event->message->data);
        return;
    }

    fprintf(stdout, "\tUnknown Message Type\n");
}

void register_connection(acc_client_t *client) {
    acc_server_request_t request;

    request.type = ACC_OUTBOUND_MESSAGE_REGISTER_COMMAND_APPLICATION;
    request.data.register_application = malloc(sizeof(acc_reg_app_t));
    request.data.register_application->update_interval = 1000;
    request.data.register_application->display_name = new_string("Y C", 0);
    request.data.register_application->connection_password = new_string("asd", 3);
    request.data.register_application->command_password = new_string("", 0);

    bool has_error = acc_client_send(client, &request);
    check_error(has_error, "register_connection");
    fprintf(stdout, "Connection has been registered...\n");
}

void unregister_connection(acc_client_t *client) {
    acc_server_request_t request;

    request.type = ACC_OUTBOUND_MESSAGE_UNREGISTER_COMMAND_APPLICATION;
    request.data.unregister_application = malloc(sizeof(acc_unreg_app_t));

    bool has_error = acc_client_send(client, &request);
    check_error(has_error, "unregister_connection");
    fprintf(stdout, "Connection has been unregistered...\n");
}

void request_entry_list(acc_client_t *client) {
    acc_server_request_t request;

    request.type = ACC_OUTBOUND_MESSAGE_REQUEST_ENTRY_LIST;
    request.data.request_entry_list->connection_id = connection_id;

    bool has_error = acc_client_send(client, &request);
    check_error(has_error, "request_entry_list");
    fprintf(stdout, "Entry list has been requested...\n");
}

void request_track_data(acc_client_t *client) {
    acc_server_request_t request;

    request.type = ACC_OUTBOUND_MESSAGE_REQUEST_TRACK_DATA;
    request.data.request_track_data->connection_id = connection_id;

    bool has_error = acc_client_send(client, &request);
    check_error(has_error, "request_track_data");
    fprintf(stdout, "Track Data has been requested...\n");
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    fprintf(stdout, "Starting...\n");

    acc_client_t *client = acc_client_create("127.0.0.1", 9000);
    check_error(client == NULL, "acc_client_create");
    fprintf(stdout, "Connection has been established...\n");

    register_connection(client);
    receive_response(client);

    request_track_data(client);
    receive_response(client);

    request_entry_list(client);
    receive_response(client);

    for (int i = 0; i < 10; i++) {
        receive_response(client);
    }

    unregister_connection(client);
    receive_response(client);

    return 0;
}
