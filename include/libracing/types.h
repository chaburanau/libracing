#pragma once

#include <stdint.h>

typedef struct {
    uint32_t size;
    char *data;
} string_t;

typedef struct {
    uint32_t size;
    int32_t *data;
} int32_array_t;

typedef struct {
    uint32_t size;
    uint16_t *data;
} uint16_array_t;

typedef struct {
    uint32_t size;
    string_t **data;
} string_array_t;

