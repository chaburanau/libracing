#include <stdbool.h>
#include <stddef.h>

#include "../include/libracing/iracing.h"

static const char *i_shared_memory = NULL;
static const iracing_header_t *i_header = NULL;

static bool initialized = false;

bool iracing_startup() {
	return false;
}

bool iracing_shutdown() {
	return false;
}

bool iracing_is_connected() {
	return false;
}

iracing_header_t iracing_get_header() {
	iracing_header_t header;
	return header;
}

iracing_session_info iracing_get_session_info() {
	iracing_session_info session_info;
	return session_info;
}

iracing_variable_header_t iracing_get_variable_header(int index) {
	iracing_variable_header_t variable_header;
	return variable_header;
}

char iracing_get_char_variable(int offset) {
	char *data = ((char*)(i_shared_memory + offset));
	return *data;
}

bool iracing_get_bool_variable(int offset) {
	bool *data = ((bool*)(i_shared_memory + offset));
	return *data;
}

int iracing_get_int_variable(int offset) {
	int *data = ((int*)(i_shared_memory + offset));
	return *data;
}

int iracing_get_bitfield_variable(int offset) {
	int *data = ((int*)(i_shared_memory + offset));
	return *data;
}

float iracing_get_float_variable(int offset) {
	float *data = ((float*)(i_shared_memory + offset));
	return *data;
}

double iracing_get_double_variable(int offset) {
	double *data = ((double*)(i_shared_memory + offset));
	return *data;
}
