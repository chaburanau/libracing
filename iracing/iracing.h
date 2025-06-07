#pragma once

static const int IRACING_VER = 2;

static const int IRACING_MAX_BUFFERS = 4;
static const int IRACING_MAX_STRING = 32;
static const int IRACING_MAX_DESC = 64;

static const int IRACING_UNLIMITED_LAPS = 32767;
static const float IRACING_UNLIMITED_TIME = 604800.0f;

static const char IRACING_DATA_VALID_EVENT_NAME[] = "Local\\IRSDKDataValidEvent";
static const char IRACING_MEM_MAP_FILE_NAME[] = "Local\\IRSDKMemMapFileName";
static const char IRACING_BROADCAST_MSG_NAME[] = "IRSDK_BROADCASTMSG";

typedef enum VariableType {
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

typedef struct VariableBuffer {
	int ticks;		// used to detect changes in data
	int offset;		// offset from header
	int padding[2];	// (16 byte align)
} iracing_variable_buffer_t;

typedef struct VariableHeader {
	int type;						// iRacing_VariableType
	int offset;						// offset from start of buffer row
	int count;						// number of entries so length in bytes would be iRacing_VariableType[type] * count
	bool as_time;					// Flag to consider the value as time
	char padding[3];				// (16 byte align)
	char name[IRACING_MAX_STRING];	// Human-readable name of the variable
	char desc[IRACING_MAX_DESC];	// Human-readable description of the variable
	char unit[IRACING_MAX_STRING];	// Units value is measured on, something like "kg/m^2"
} iracing_variable_header_t;

typedef struct SessionInfo {
	int version;
	char *data;
} iracing_session_info;

typedef struct Header {
	// global information
	int version;			// this api header version, see IRACING_VER
	int status;				// bitfield using irsdk_StatusField
	int tick_rate;			// ticks per second (60 or 360 etc)

	// session information, updated periodicaly
	int session_info_update;	// Incremented when session info changes
	int session_info_length;	// Length in bytes of session info string
	int session_info_offset;	// Session info, encoded in YAML format

	// state data, output at tickRate
	int number_of_variables;		// length of array pointed to by varHeaderOffset
	int variables_header_offset;	// offset to irsdk_varHeader[numVars] array, Describes the variables received in varBuf

	int number_of_buffers;	// <= IRACING_MAX_BUFS (3 for now)
	int buffer_length;		// length in bytes for one line
	//****ToDo, add these in
	//int curBufTickCount;	// stashed copy of the current tickCount, can read this to see if new data is available
	//byte curBuf;			// index of the most recently written buffer (0 to IRSDK_MAX_BUFS-1)
	//byte pad1[3];			// 16 byte align
	int padding1[2];		// (16 byte align)
	VariableBuffer variable_buffers[IRACING_MAX_BUFFERS]; // buffers of data being written to
} iracing_header_t;

bool iracing_startup();
bool iracing_shutdown();
bool iracing_is_connected();

iracing_header_t iracing_get_header();
iracing_session_info iracing_get_session_info();
iracing_variable_header_t iracing_get_variable_header(int index);

char iracing_get_char_variable(int index);
bool iracing_get_bool_variable(int index);
int iracing_get_int_variable(int index);
int iracing_get_bitfield_variable(int index);
float iracing_get_float_variable(int index);
double iracing_get_double_variable(int index);
