#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DEFINE_ARRAY_TYPE(TYPE, NAME)                \
    typedef struct {                                 \
        TYPE *data;                                  \
        size_t size;                                 \
    } NAME;                                          \
                                                     \
    static inline NAME *NAME##_create(size_t size) { \
        NAME *array = malloc(sizeof(NAME));          \
        if (array == NULL) {                         \
            return NULL;                             \
        }                                            \
                                                     \
        array->data = malloc(sizeof(TYPE) * size);   \
        if (array->data == NULL) {                   \
            free(array);                             \
            return NULL;                             \
        }                                            \
                                                     \
        array->size = size;                          \
        return array;                                \
    }                                                \
                                                     \
    static inline void NAME##_destroy(NAME *array) { \
        if (array == NULL) {                         \
            return;                                  \
        }                                            \
                                                     \
        free(array->data);                           \
        free(array);                                 \
        array = NULL;                                \
    }

DEFINE_ARRAY_TYPE(char, string_t)
DEFINE_ARRAY_TYPE(string_t, strings_t)
DEFINE_ARRAY_TYPE(int32_t, int32s_t)
DEFINE_ARRAY_TYPE(uint16_t, uint16s_t)
