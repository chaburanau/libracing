#include "../src/iracing/client.c"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void check_error(bool status, char *operation) {
    if (status == false) {
        fprintf(stderr, "%s(...) failed with error %d\n", operation, iracing_get_last_error());
        exit(-1);
    }
}

void print_header(iracing_header_t *header) {
    fprintf(stdout, "Header:\n");

    fprintf(stdout, "\tVersion: %d\n", header->version);
    fprintf(stdout, "\tStatus: %d\n", header->status);
    fprintf(stdout, "\tTick Rate: %d\n", header->tick_rate);

    fprintf(stdout, "\tSession Info Version: %d\n", header->session_info_version);
    fprintf(stdout, "\tSession Info Length: %d\n", header->session_info_length);
    fprintf(stdout, "\tSession Info Offset: %d\n", header->session_info_offset);

    fprintf(stdout, "\tNumber of Variables: %d\n", header->number_of_variables);
    fprintf(stdout, "\tVariables Info Offset: %d\n", header->variables_info_offset);

    fprintf(stdout, "\tNumber of Buffers: %d\n", header->number_of_buffers);
    fprintf(stdout, "\tBuffer Length: %d\n", header->buffer_length);

    for (int32_t index = 0; index < header->number_of_buffers; index++) {
        fprintf(stdout, "\tBuffer Tick: %d\n", header->variable_buffers[index].tick);
        fprintf(stdout, "\tBuffer Offset: %d\n", header->variable_buffers[index].offset);
    }

    fprintf(stdout, "---\n");
}

void print_session_info(iracing_session_info_t *info) {
    fprintf(stdout, "Session Info:\n");

    fprintf(stdout, "\tVersion: %d\n", info->version);
    fprintf(stdout, "\tData: %s\n", info->data.data);

    fprintf(stdout, "---\n");
}

void print_variable_info(iracing_variable_info_t *info) {

    fprintf(stdout, "\tType: %d\n", info->type);
    fprintf(stdout, "\tOffset: %d\n", info->offset);
    fprintf(stdout, "\tCount: %d\n", info->count);
    fprintf(stdout, "\tAs Time: %d\n", info->as_time);

    fprintf(stdout, "\tName: %s\n", info->name);
    fprintf(stdout, "\tDesc: %s\n", info->desc);
    fprintf(stdout, "\tUnit: %s\n", info->unit);
}

void print_variable(iracing_variable_info_t *info, void *data) {
    switch (info->type) {
    case IRACING_TYPE_CHAR:
        fprintf(stdout, "\tChar Single: %c\n", *((char *)data));
        break;
    case IRACING_TYPE_BOOL:
        fprintf(stdout, "\tBool Single: %d\n", *((bool *)data));
        break;
    case IRACING_TYPE_INT:
        fprintf(stdout, "\tInt Single: %d\n", *((int32_t *)data));
        break;
    case IRACING_TYPE_BITFIELD:
        fprintf(stdout, "\tBitField Single: %d\n", *((int32_t *)data));
        break;
    case IRACING_TYPE_FLOAT:
        fprintf(stdout, "\tFloat Single: %f\n", *((float *)data));
        break;
    case IRACING_TYPE_DOUBLE:
        fprintf(stdout, "\tDouble Single: %f\n", *((double *)data));
        break;
    case IRACING_TYPE_ETCOUNT:
        fprintf(stdout, "\tETCount Single: %lld\n", *((int64_t *)data));
        break;
    }
}

void read_variables(iracing_client_t *client, iracing_header_t *header) {
    bool status;

    iracing_variable_info_t variable_info;

    for (int32_t index = 0; index < header->number_of_variables; index++) {
        fprintf(stdout, "Variable:\n");
        void *data = NULL;

        status = iracing_read_variable_info(client, header, &variable_info, (size_t)index);
        check_error(status, "iracing_read_variable_info");
        print_variable_info(&variable_info);

        status = iracing_read_variable_value(client, header, &variable_info, &data);
        check_error(status, "iracing_read_variable_value");
        print_variable(&variable_info, data);

        free(data);
        fprintf(stdout, "---\n");
    }
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    iracing_client_t client;
    iracing_header_t header;
    iracing_session_info_t info;

    bool status;

    status = iracing_client_init(&client);
    check_error(status, "iracing_client_init");

    status = iracing_read_header(&client, &header);
    check_error(status, "iracing_read_header");
    print_header(&header);

    status = iracing_read_session_info(&client, &header, &info);
    check_error(status, "iracing_read_session_info");
    print_session_info(&info);

    read_variables(&client, &header);

    status = iracing_client_free(&client);
    check_error(status, "iracing_client_free");

    return 0;
}
