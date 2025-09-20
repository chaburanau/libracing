#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DEFINE_ARRAY_TYPE(TYPE, NAME)              \
    typedef struct {                               \
        TYPE *data;                                \
        size_t size;                               \
    } NAME;                                        \
                                                   \
    bool NAME##_init(NAME *array, size_t size) {  \
        if (array == NULL) {                       \
            return true;                           \
        }                                          \
                                                   \
        array->size = size;                        \
        array->data = malloc(sizeof(TYPE) * size); \
                                                   \
        if (array->data == NULL) {                 \
            return false;                          \
        }                                          \
                                                   \
        return true;                               \
    }                                              \
                                                   \
    void NAME##_free(NAME *array) {                \
        if (array == NULL) {                       \
            return;                                \
        }                                          \
        if (array->data != NULL) {                 \
            free(array->data);                     \
            array->data = NULL;                    \
        }                                          \
    }

DEFINE_ARRAY_TYPE(char, string_t)
DEFINE_ARRAY_TYPE(string_t, strings_t)
DEFINE_ARRAY_TYPE(int32_t, int32s_t)
DEFINE_ARRAY_TYPE(uint16_t, uint16s_t)

void strings_free(strings_t *array) {
    if (array == NULL || array->data == NULL) {
        return;
    }

    for (size_t index = 0; index < array->size; index++) {
        string_t_free(&array->data[index]);
    }

    strings_t_free(array);
}
