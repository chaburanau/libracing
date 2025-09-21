#pragma once

#include "../utils/types.c"
#include <stdbool.h>
#include <stdint.h>

#define IRACING_VERSION 2
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

    // index, don't use
    IRACING_TYPE_ETCOUNT,
} iracing_variable_type_t;

typedef enum {
    IRACING_TRACK_LOCATION_NOT_IN_WORLD = -1,
    IRACING_TRACK_LOCATION_OFF_TRACK = 0,
    IRACING_TRACK_LOCATION_IN_PIT_STALLS = 1,
    IRACING_TRACK_LOCATION_APPROACHING_PITS = 2,
    IRACING_TRACK_LOCATION_ON_TRACK = 3,
} iracing_track_location;

typedef enum {
    IRACING_TRACK_SURFACE_NOT_IN_WORLD = -1,
    IRACING_TRACK_SURFACE_UNDEFINED_MATERIAL = 0,
    IRACING_TRACK_SURFACE_ASPHALT_MATERIAL_1 = 1,
    IRACING_TRACK_SURFACE_ASPHALT_MATERIAL_2 = 2,
    IRACING_TRACK_SURFACE_ASPHALT_MATERIAL_3 = 3,
    IRACING_TRACK_SURFACE_ASPHALT_MATERIAL_4 = 4,
    IRACING_TRACK_SURFACE_CONCRETE_MATERIAL_1 = 5,
    IRACING_TRACK_SURFACE_CONCRETE_MATERIAL_2 = 6,
    IRACING_TRACK_SURFACE_RACING_DIRT_MATERIAL_1 = 7,
    IRACING_TRACK_SURFACE_RACING_DIRT_MATERIAL_2 = 8,
    IRACING_TRACK_SURFACE_PAINT_MATERIAL_1 = 9,
    IRACING_TRACK_SURFACE_PAINT_MATERIAL_2 = 10,
    IRACING_TRACK_SURFACE_RUMBLE_MATERIAL_1 = 11,
    IRACING_TRACK_SURFACE_RUMBLE_MATERIAL_2 = 12,
    IRACING_TRACK_SURFACE_RUMBLE_MATERIAL_3 = 13,
    IRACING_TRACK_SURFACE_RUMBLE_MATERIAL_4 = 14,

    IRACING_TRACK_SURFACE_GRASS_MATERIAL_1 = 15,
    IRACING_TRACK_SURFACE_GRASS_MATERIAL_2 = 16,
    IRACING_TRACK_SURFACE_GRASS_MATERIAL_3 = 17,
    IRACING_TRACK_SURFACE_GRASS_MATERIAL_4 = 18,
    IRACING_TRACK_SURFACE_DIRT_MATERIAL_1 = 19,
    IRACING_TRACK_SURFACE_DIRT_MATERIAL_2 = 20,
    IRACING_TRACK_SURFACE_DIRT_MATERIAL_3 = 21,
    IRACING_TRACK_SURFACE_DIRT_MATERIAL_4 = 22,
    IRACING_TRACK_SURFACE_SAND_MATERIAL = 23,
    IRACING_TRACK_SURFACE_GRAVEL_MATERIAL_1 = 24,
    IRACING_TRACK_SURFACE_GRAVEL_MATERIAL_2 = 25,
    IRACING_TRACK_SURFACE_GRASSCRETE_MATERIAL = 26,
    IRACING_TRACK_SURFACE_ASTROTURF_MATERIAL = 27,
} iracing_track_surface;

typedef enum {
    IRACING_SESSION_STATE_INVALID = 0,
    IRACING_SESSION_STATE_GET_IN_CAR = 1,
    IRACING_SESSION_STATE_WARMUP = 2,
    IRACING_SESSION_STATE_PARADE_LAPS = 3,
    IRACING_SESSION_STATE_RACING = 4,
    IRACING_SESSION_STATE_CHECKERED = 5,
    IRACING_SESSION_STATE_COOL_DOWN = 6,
} iracing_session_state;

typedef enum {
    IRACING_CARS_AROUND_OFF = 0,
    IRACING_CARS_AROUND_CLEAR = 1,
    IRACING_CARS_AROUND_LEFT = 2,
    IRACING_CARS_AROUND_RIGHT = 3,
    IRACING_CARS_AROUND_LEFT_RIGHT = 4,
    IRACING_CARS_AROUND_LEFT_2 = 5,
    IRACING_CARS_AROUND_RIGHT_2 = 6,
} iracing_cars_around;

typedef enum {
    IRACING_PIT_STATUS_NONE = 0,
    IRACING_PIT_STATUS_IN_PROGRESS = 1,
    IRACING_PIT_STATUS_COMPLETE = 2,

    IRACING_PIT_STATUS_TOO_FAR_LEFT = 3,
    IRACING_PIT_STATUS_TOO_FAR_RIGHT = 4,
    IRACING_PIT_STATUS_TOO_FAR_FORWARD = 5,
    IRACING_PIT_STATUS_TOO_FAR_BACK = 6,
    IRACING_PIT_STATUS_BAD_ANGLE = 7,
    IRACING_PIT_STATUS_CANT_FIX_THAT = 8,
} iracing_pit_status;

typedef enum {
    IRACING_PACE_MODE_SINGLE_FILE_START = 0,
    IRACING_PACE_MODE_DOUBLE_FILE_START = 1,
    IRACING_PACE_MODE_SINGLE_FILE_RESTART = 2,
    IRACING_PACE_MODE_DOUBLE_FILE_RESTART = 3,
    IRACING_PACE_MODE_NOT_PACING = 4,
} iracing_pace_mode;

typedef enum {
    IRACING_TRACK_WETNESS_UNKNOWN = 0,
    IRACING_TRACK_WETNESS_DRY = 1,
    IRACING_TRACK_WETNESS_MOSTLY_DRY = 2,
    IRACING_TRACK_WETNESS_VERY_LIGHTLY_WET = 3,
    IRACING_TRACK_WETNESS_LIGHTLY_WET = 4,
    IRACING_TRACK_WETNESS_MODERATELY_WET = 5,
    IRACING_TRACK_WETNESS_VERY_WET = 6,
    IRACING_TRACK_WETNESS_EXTREMELY_WET = 7,
} iracing_track_wetness;

typedef enum {
    // First byte is incident report flag
    // Only one of these will be used
    IRACING_INCIDENT_FLAG_REPORT_NO_REPORT = 0x0000,                    // No penalty
    IRACING_INCIDENT_FLAG_REPORT_OUT_OF_CONTROL = 0x0001,               // "Loss of Control (2x)"
    IRACING_INCIDENT_FLAG_REPORT_OFF_TRACK = 0x0002,                    // "Off Track (1x)"
    IRACING_INCIDENT_FLAG_REPORT_OFF_TRACK_ONGOING = 0x0003,            // Not currently sent
    IRACING_INCIDENT_FLAG_REPORT_CONTACT_WITH_WORLD = 0x0004,           // "Contact (0x)"
    IRACING_INCIDENT_FLAG_REPORT_COLLISION_WITH_WORLD = 0x0005,         // "Contact (2x)"
    IRACING_INCIDENT_FLAG_REPORT_COLLISION_WITH_WORLD_ONGOING = 0x0006, // Not currently sent
    IRACING_INCIDENT_FLAG_REPORT_CONTACT_WITH_CAR = 0x0007,             // "Car Contact (0x)"
    IRACING_INCIDENT_FLAG_REPORT_COLLISION_WITH_CAR = 0x0008,           // "Car Contact (4x)"

    // Second byte is incident penalty
    // Only one of these will be used
    IRACING_INCIDENT_FLAG_PENALTY_NO_REPORT = 0x0000, // No penalty
    IRACING_INCIDENT_FLAG_PENALTY_ZERO_X = 0x0100,    // 0x
    IRACING_INCIDENT_FLAG_PENALTY_ONE_X = 0x0200,     // 1x
    IRACING_INCIDENT_FLAG_PENALTY_TWO_X = 0x0300,     // 2x
    IRACING_INCIDENT_FLAG_PENALTY_FOUR_X = 0x0400,    // 4x

    // Not enums, used to seperate the above incident report field
    // From the incident penalty field
    IRSDK_INCIDENT_REP_MASK = 0x000000FF,
    IRSDK_INCIDENT_PEN_MASK = 0x0000FF00,
} iracing_incident_flags;

typedef enum {
    IRACING_ENGINE_WARNING_WATER_TEMP = 0x0001,
    IRACING_ENGINE_WARNING_FUEL_PRESSURE = 0x0002,
    IRACING_ENGINE_WARNING_OIL_PRESSURE = 0x0004,
    IRACING_ENGINE_WARNING_ENGINE_STALLED = 0x0008,
    IRACING_ENGINE_WARNING_PIT_SPEED_LIMITER = 0x0010,
    IRACING_ENGINE_WARNING_REV_LIMITER_ACTIVATED = 0x0020,
    IRACING_ENGINE_WARNING_OIL_TEMP = 0x0040,
    IRACING_ENGINE_WARNING_MANDATORY_REPAIRS_NEEDED = 0x0080, // Car needs mandatory repairs
    IRACING_ENGINE_WARNING_OPTIONAL_REPAIRS_NEEDED = 0x0100,  // Car needs optional repairs
} iracing_engine_warning;

typedef enum {
    // Global flags
    IRACING_FLAGS_CHECKERED = 0x00000001,
    IRACING_FLAGS_WHITE = 0x00000002,
    IRACING_FLAGS_GREEN = 0x00000004,
    IRACING_FLAGS_YELLOW = 0x00000008,
    IRACING_FLAGS_RED = 0x00000010,
    IRACING_FLAGS_BLUE = 0x00000020,
    IRACING_FLAGS_DEBRIS = 0x00000040,
    IRACING_FLAGS_CROSSED = 0x00000080,
    IRACING_FLAGS_YELLOW_WAVING = 0x00000100,
    IRACING_FLAGS_ONE_LAP_TO_GREEN = 0x00000200,
    IRACING_FLAGS_GREEN_HELP = 0x00000400,
    IRACING_FLAGS_TEN_TO_GO = 0x00000800,
    IRACING_FLAGS_FIVE_TO_GO = 0x00001000,
    IRACING_FLAGS_RANDOM_WAVING = 0x00002000,
    IRACING_FLAGS_CAUTION = 0x00004000,
    IRACING_FLAGS_CAUTION_WAVING = 0x00008000,

    // Drivers black flags
    IRACING_FLAGS_BLACK = 0x00010000,
    IRACING_FLAGS_DISQUALIFY = 0x00020000,
    IRACING_FLAGS_SERVICIBLE = 0x00040000, // Car is allowed service (not a flag)
    IRACING_FLAGS_FURLED = 0x00080000,
    IRACING_FLAGS_REPAIR = 0x00100000,
    IRACING_FLAGS_DISQUALIFY_SCORING_DISABLED = 0x00200000, // Car is disqualified and scoring is disabled

    // Start lights
    IRACING_FLAGS_START_HIDDEN = 0x10000000,
    IRACING_FLAGS_START_READY = 0x20000000,
    IRACING_FLAGS_START_SET = 0x40000000,
    IRACING_FLAGS_START_GO = 0x80000000,
} iracing_flags;

typedef enum {
    IRACING_CAMERA_STATE_IS_SESSION_SCREEN = 0x0001, // The camera tool can only be activated if viewing the session screen
    IRACING_CAMERA_STATE_IS_SCENIC_ACTIVE = 0x0002,  // The scenic camera is active (no focus car)

    // These can be changed with a broadcast message
    IRACING_CAMERA_TOOL_ACTIVE = 0x0004,
    IRACING_UI_HIDDEN = 0x0008,
    IRACING_USE_AUTO_SHOT_SELECTION = 0x0010,
    IRACING_USE_TEMPORARY_EDITS = 0x0020,
    IRACING_USE_KEY_ACCELERATION = 0x0040,
    IRACING_USE_KEY_10X_ACCELERATION = 0x0080,
    IRACING_USE_MOUSE_AIM_MODE = 0x0100
} iracing_camera_state_t;

typedef enum {
    IRACING_PIT_FLAGS_LEFT_FRONT_TIRE_CHANGE = 0x0001,
    IRACING_PIT_FLAGS_RIGHT_FRONT_TIRE_CHANGE = 0x0002,
    IRACING_PIT_FLAGS_LEFT_REAR_TIRE_CHANGE = 0x0004,
    IRACING_PIT_FLAGS_RIGHT_READ_TIRE_CHANGE = 0x0008,

    IRACING_PIT_FLAGS_FUEL_FILL = 0x0010,
    IRACING_PIT_FLAGS_WINDSHIELD_TEAROFF = 0x0020,
    IRACING_PIT_FLAGS_FAST_REPAIR = 0x0040
} iracing_pit_flags_t;

typedef enum {
    IRACING_PACE_FLAGS_END_OF_LINE = 0x0001,
    IRACING_PACE_FLAGS_FREE_PASS = 0x0002,
    IRACING_PACE_FLAGS_WAVED_AROUND = 0x0004,
} iracing_pace_flags;

typedef enum {
    IRACING_STATUS_FIELD_CONNECTED = 1,
} iracing_status_field_t;

typedef struct {
    int32_t tick;       // Version of the data
    int32_t offset;     // Offset from header
    int32_t padding[2]; // (16 byte align)
} iracing_variable_buffer_t;

typedef struct {
    int32_t type;                  // Iracing_variable_type_t
    int32_t offset;                // Offset from start of buffer row
    int32_t count;                 // Number of entries so length in bytes would be iracing_variable_type_t[type] * count
    bool as_time;                  // Flag to consider the value as time
    char padding[3];               // (16 byte align)
    char name[IRACING_MAX_STRING]; // Human-readable name of the variable
    char desc[IRACING_MAX_DESC];   // Human-readable description of the variable
    char unit[IRACING_MAX_STRING]; // Units value is measured on, something like "kg/m^2"
} iracing_variable_header_t;

typedef struct {
    int32_t version; // Session info version
    string_t *data;  // YAML formatted session info data
} iracing_session_info_t;

typedef struct {
    // Global information
    int32_t version;   // This api header version, see IRACING_VERSION
    int32_t status;    // Bitfield using iracing_status_field_t
    int32_t tick_rate; // Ticks per second (60 or 360 etc)

    // Session information, updated periodicaly
    int32_t session_info_update; // Incremented when session info changes
    int32_t session_info_length; // Length in bytes of session info string
    int32_t session_info_offset; // Session info, encoded in YAML format

    // State data, output at tick rate
    int32_t number_of_variables;     // Length of array pointed to by varHeaderOffset
    int32_t variables_header_offset; // Offset to iracing_variable_header_t array

    int32_t number_of_buffers;                                       // <= IRACING_MAX_BUFFERS (3 for now)
    int32_t buffer_length;                                           // Length in bytes for one line
    int32_t padding[2];                                              // (16 byte align)
    iracing_variable_buffer_t variable_buffers[IRACING_MAX_BUFFERS]; // Buffers of data being written to
} iracing_header_t;
