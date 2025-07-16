#pragma once

#include <cstdint>
#include <stdbool.h>
#include "types.h"

#define IRACING_VER  2
#define IRACING_MAX_BUFFERS 4
#define IRACING_MAX_STRING 32
#define IRACING_MAX_DESC 64

#define IRACING_UNLIMITED_LAPS 32767
#define IRACING_UNLIMITED_TIME 604800.0f

const char IRACING_DATA_VALID_EVENT_NAME[] = "Local\\IRSDKDataValidEvent";
const char IRACING_MEM_MAP_FILE_NAME[] = "Local\\IRSDKMemMapFileName";
const char IRACING_BROADCAST_MSG_NAME[] = "IRSDK_BROADCASTMSG";

typedef enum {
	// 1 byte
	IRACING_TYPE_CHAR = 0,
	IRACING_TYPE_BOOL,

	// 4 bytes
	IRACING_TYPE_INT,
	IRACING_TYPE_BITFIELD,
	IRACING_TYPE_FLOAT,

	// 8 bytes
	IRACING_TYPE_DOUBLE,

	//index, don't use
	IRACING_TYPE_ETCOUNT,
} iracing_variable_type_t;

typedef struct {
	int32_t ticks;		// used to detect changes in data
	int32_t offset;		// offset from header
	int32_t padding[2];	// (16 byte align)
} iracing_variable_buffer_t;

typedef struct {
	int32_t type;						// iRacing_VariableType
	int32_t offset;						// offset from start of buffer row
	int32_t count;						// number of entries so length in bytes would be iRacing_VariableType[type] * count
	bool as_time;					// Flag to consider the value as time
	char padding[3];				// (16 byte align)
	char name[IRACING_MAX_STRING];	// Human-readable name of the variable
	char desc[IRACING_MAX_DESC];	// Human-readable description of the variable
	char unit[IRACING_MAX_STRING];	// Units value is measured on, something like "kg/m^2"
} iracing_variable_header_t;

typedef struct {
	int32_t version;
	string_t *data;
} iracing_session_info_t;

typedef struct {
	// global information
	int32_t version;			// this api header version, see IRACING_VER
	int32_t status;				// bitfield using irsdk_StatusField
	int32_t tick_rate;			// ticks per second (60 or 360 etc)

	// session information, updated periodicaly
	int32_t session_info_update;	// Incremented when session info changes
	int32_t session_info_length;	// Length in bytes of session info string
	int32_t session_info_offset;	// Session info, encoded in YAML format

	// state data, output at tickRate
	int32_t number_of_variables;		// length of array pointed to by varHeaderOffset
	int32_t variables_header_offset;	// offset to irsdk_varHeader[numVars] array, Describes the variables received in varBuf

	int32_t number_of_buffers;	// <= IRACING_MAX_BUFS (3 for now)
	int32_t buffer_length;		// length in bytes for one line
	//****ToDo, add these in
	//int curBufTickCount;	// stashed copy of the current tickCount, can read this to see if new data is available
	//byte curBuf;			// index of the most recently written buffer (0 to IRSDK_MAX_BUFS-1)
	//byte pad1[3];			// 16 byte align
	int32_t padding1[2];		// (16 byte align)
	iracing_variable_buffer_t variable_buffers[IRACING_MAX_BUFFERS]; // buffers of data being written to
} iracing_header_t;

typedef struct iRacingClient iracing_client_t;

iracing_client_t *iracing_connect();
void iracing_disconnect(iracing_client_t *client);

iracing_header_t iracing_get_header(iracing_client_t *client);
iracing_session_info_t iracing_get_session_info(iracing_client_t *client);
iracing_variable_header_t iracing_get_variable_header(iracing_client_t *client, int32_t index);

char iracing_get_char_variable(iracing_client_t *client, int offset);
bool iracing_get_bool_variable(iracing_client_t *client, int offset);
int32_t iracing_get_int_variable(iracing_client_t *client, int offset);
int32_t iracing_get_bitfield_variable(iracing_client_t *client, int offset);
float iracing_get_float_variable(iracing_client_t *client, int offset);
double iracing_get_double_variable(iracing_client_t *client, int offset);
