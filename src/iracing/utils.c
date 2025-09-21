#include "./types.c"
#include <stddef.h>
#include <stdint.h>

size_t iracing_get_latest_tick(iracing_header_t *header) {
    size_t latest = 0;

    for (size_t index = 0; index < (size_t)header->number_of_buffers; index++) {
        if (header->variable_buffers[index].tick > header->variable_buffers[latest].tick) {
            latest = index;
        }
    }

    return latest;
}

size_t iracing_variable_size(iracing_variable_type_t type) {
    switch (type) {
    case IRACING_TYPE_CHAR:
        return sizeof(char);
    case IRACING_TYPE_BOOL:
        return sizeof(char);
    case IRACING_TYPE_INT:
        return sizeof(int32_t);
    case IRACING_TYPE_BITFIELD:
        return sizeof(int32_t);
    case IRACING_TYPE_FLOAT:
        return sizeof(float);
    case IRACING_TYPE_DOUBLE:
        return sizeof(double);
    case IRACING_TYPE_ETCOUNT:
        return sizeof(int64_t);
    }
}
