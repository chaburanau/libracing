#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "types.h"

#define ACC_BROADCASTING_PROTOCOL_VERSION 4

typedef enum {
    ACC_STATUS_OK = 0,
    ACC_STATUS_NOT_INITIALIZED = 1,
    ACC_STATUS_NOT_HANDSHAKED = 2,
    ACC_STATUS_NOT_CONNECTED = 3,
    ACC_STATUS_NOT_REGISTERED = 4,
    ACC_STATUS_NOT_READY = 5,
    ACC_STATUS_NOT_RUNNING = 6,
    ACC_STATUS_NOT_PAUSED = 7,
    ACC_STATUS_NOT_FINISHED = 8,
    ACC_STATUS_ALREADY_INITIALIZED = 9,
    ACC_STATUS_SOCKET_ERROR = 10,
    ACC_STATUS_NOT_DISMISSED = 11,
    ACC_STATUS_CLIENT_NOT_INITIALIZED = 100,
    ACC_STATUS_SEND_ERROR = 101,
    ACC_STATUS_RECEIVE_ERROR = 102,
    ACC_STATUS_INVALID_OUTBOUND_MESSAGE_TYPE = 200,
    ACC_STATUS_INVALID_INBOUND_MESSAGE_TYPE = 201,
    ACC_STATUS_BUFFER_SIZE_MISMATCH = 300,
    ACC_STATUS_UNEXPECTED_AMOUNT_OF_BYTES_SENT = 400,
    ACC_STATUS_UNEXPECTED_AMOUNT_OF_BYTES_RECEIVED = 500,
    ACC_STATUS_INBOUND_BUFFER_TOO_SMALL = 600,
    ACC_STATUS_MEMORY_ALLOCATION_ERROR = 1000,
} acc_status_t;

typedef enum {
    ACC_LAP_TYPE_INVALID = 0,
    ACC_LAP_TYPE_OUTLAP = 1,
    ACC_LAP_TYPE_REGULAR = 2,
    ACC_LAP_TYPE_INLAP = 3,
} acc_lap_type_t;

typedef enum {
    ACC_DRIVER_CATEGORY_BRONZE = 0,
    ACC_DRIVER_CATEGORY_SILVER = 1,
    ACC_DRIVER_CATEGORY_GOLD = 2,
    ACC_DRIVER_CATEGORY_PLATINUM = 3,
    ACC_DRIVER_CATEGORY_UNKNOWN = 255,
} acc_driver_category_t;

typedef enum {
    ACC_CAR_LOCATION_UNKNOWN = 0,
    ACC_CAR_LOCATION_TRACK = 1,
    ACC_CAR_LOCATION_PITLANE = 2,
    ACC_CAR_LOCATION_PITENTRY = 3,
    ACC_CAR_LOCATION_PITEXIT = 4,
} acc_car_location_t;

typedef enum {
    ACC_SESSION_PHASE_UNKNOWN = 0,
    ACC_SESSION_PHASE_STARTING = 1,
    ACC_SESSION_PHASE_PRE_FORMATION = 2,
    ACC_SESSION_PHASE_FORMATION_LAP = 3,
    ACC_SESSION_PHASE_PRE_SESSION = 4,
    ACC_SESSION_PHASE_SESSION = 5,
    ACC_SESSION_PHASE_SESSION_OVER = 6,
    ACC_SESSION_PHASE_POST_SESSION = 7,
    ACC_SESSION_PHASE_RESULT_UI = 8,
} acc_session_phase_t;

typedef enum {
    ACC_SESSION_TYPE_PRACTICE = 0,
    ACC_SESSION_TYPE_QUALIFYING = 4,
    ACC_SESSION_TYPE_SUPERPOLE = 9,
    ACC_SESSION_TYPE_RACE = 10,
    ACC_SESSION_TYPE_HOT_LAP = 11,
    ACC_SESSION_TYPE_HOT_STINT = 12,
    ACC_SESSION_TYPE_HOT_LAP_SUPERPOLE = 13,
    ACC_SESSION_TYPE_REPLAY = 14,
} acc_session_type_t;

typedef enum {
    ACC_BROADCASTING_EVENT_TYPE_UNKNOWN = 0,
    ACC_BROADCASTING_EVENT_TYPE_GREEN_FLAG = 1,
    ACC_BROADCASTING_EVENT_TYPE_SESSION_OVER = 2,
    ACC_BROADCASTING_EVENT_TYPE_PENALTY_COMMUNICATION_MESSAGE = 3,
    ACC_BROADCASTING_EVENT_TYPE_ACCIDENT = 4,
    ACC_BROADCASTING_EVENT_TYPE_LAP_COMPLETED = 5,
    ACC_BROADCASTING_EVENT_TYPE_BEST_SESSION_LAP = 6,
    ACC_BROADCASTING_EVENT_TYPE_BEST_PERSONAL_LAP = 7,
} acc_broadcasting_event_type_t;

typedef enum {
    ACC_NATIONALITY_ANY = 0,
    ACC_NATIONALITY_ITALY = 1,
    ACC_NATIONALITY_GERMANY = 2,
    ACC_NATIONALITY_FRANCE = 3,
    ACC_NATIONALITY_SPAIN = 4,
    ACC_NATIONALITY_GREAT_BRITAIN = 5,
    ACC_NATIONALITY_HUNGARY = 6,
    ACC_NATIONALITY_BELGIUM = 7,
    ACC_NATIONALITY_SWITZERLAND = 8,
    ACC_NATIONALITY_AUSTRIA = 9,
    ACC_NATIONALITY_RUSSIA = 10,
    ACC_NATIONALITY_THAILAND = 11,
    ACC_NATIONALITY_NETHERLANDS = 12,
    ACC_NATIONALITY_POLAND = 13,
    ACC_NATIONALITY_ARGENTINA = 14,
    ACC_NATIONALITY_MONACO = 15,
    ACC_NATIONALITY_IRELAND = 16,
    ACC_NATIONALITY_BRAZIL = 17,
    ACC_NATIONALITY_SOUTH_AFRICA = 18,
    ACC_NATIONALITY_PUERTO_RICO = 19,
    ACC_NATIONALITY_SLOVAKIA = 20,
    ACC_NATIONALITY_OMAN = 21,
    ACC_NATIONALITY_GREECE = 22,
    ACC_NATIONALITY_SAUDI_ARABIA = 23,
    ACC_NATIONALITY_NORWAY = 24,
    ACC_NATIONALITY_TURKEY = 25,
    ACC_NATIONALITY_SOUTH_KOREA = 26,
    ACC_NATIONALITY_LEBANON = 27,
    ACC_NATIONALITY_ARMENIA = 28,
    ACC_NATIONALITY_MEXICO = 29,
    ACC_NATIONALITY_SWEDEN = 30,
    ACC_NATIONALITY_FINLAND = 31,
    ACC_NATIONALITY_DENMARK = 32,
    ACC_NATIONALITY_CROATIA = 33,
    ACC_NATIONALITY_CANADA = 34,
    ACC_NATIONALITY_CHINA = 35,
    ACC_NATIONALITY_PORTUGAL = 36,
    ACC_NATIONALITY_SINGAPORE = 37,
    ACC_NATIONALITY_INDONESIA = 38,
    ACC_NATIONALITY_USA = 39,
    ACC_NATIONALITY_NEW_ZEALAND = 40,
    ACC_NATIONALITY_AUSTRALIA = 41,
    ACC_NATIONALITY_SAN_MARINO = 42,
    ACC_NATIONALITY_UAE = 43,
    ACC_NATIONALITY_LUXEMBOURG = 44,
    ACC_NATIONALITY_KUWAIT = 45,
    ACC_NATIONALITY_HONG_KONG = 46,
    ACC_NATIONALITY_COLOMBIA = 47,
    ACC_NATIONALITY_JAPAN = 48,
    ACC_NATIONALITY_ANDORRA = 49,
    ACC_NATIONALITY_AZERBAIJAN = 50,
    ACC_NATIONALITY_BULGARIA = 51,
    ACC_NATIONALITY_CUBA = 52,
    ACC_NATIONALITY_CZECH_REPUBLIC = 53,
    ACC_NATIONALITY_ESTONIA = 54,
    ACC_NATIONALITY_GEORGIA = 55,
    ACC_NATIONALITY_INDIA = 56,
    ACC_NATIONALITY_ISRAEL = 57,
    ACC_NATIONALITY_JAMAICA = 58,
    ACC_NATIONALITY_LATVIA = 59,
    ACC_NATIONALITY_LITHUANIA = 60,
    ACC_NATIONALITY_MACAU = 61,
    ACC_NATIONALITY_MALAYSIA = 62,
    ACC_NATIONALITY_NEPAL = 63,
    ACC_NATIONALITY_NEW_CALEDONIA = 64,
    ACC_NATIONALITY_NIGERIA = 65,
    ACC_NATIONALITY_NORTHERN_IRELAND = 66,
    ACC_NATIONALITY_PAPUA_NEW_GUINEA = 67,
    ACC_NATIONALITY_PHILIPPINES = 68,
    ACC_NATIONALITY_QATAR = 69,
    ACC_NATIONALITY_ROMANIA = 70,
    ACC_NATIONALITY_SCOTLAND = 71,
    ACC_NATIONALITY_SERBIA = 72,
    ACC_NATIONALITY_SLOVENIA = 73,
    ACC_NATIONALITY_TAIWAN = 74,
    ACC_NATIONALITY_UKRAINE = 75,
    ACC_NATIONALITY_VENEZUELA = 76,
    ACC_NATIONALITY_WALES = 77,
    ACC_NATIONALITY_IRAN = 78,
    ACC_NATIONALITY_BAHRAIN = 79,
    ACC_NATIONALITY_ZIMBABWE = 80,
    ACC_NATIONALITY_CHINESE_TAIPEI = 81,
    ACC_NATIONALITY_CHILE = 82,
    ACC_NATIONALITY_URUGUAY = 83,
    ACC_NATIONALITY_MADAGASCAR = 84,
} acc_nationality_t;

typedef enum {
    ACC_OUTBOUND_MESSAGE_REGISTER_COMMAND_APPLICATION = 1,
    ACC_OUTBOUND_MESSAGE_UNREGISTER_COMMAND_APPLICATION = 9,
    ACC_OUTBOUND_MESSAGE_REQUEST_ENTRY_LIST = 10,
    ACC_OUTBOUND_MESSAGE_REQUEST_TRACK_DATA = 11,
    ACC_OUTBOUND_MESSAGE_CHANGE_HUD_PAGE = 49,
    ACC_OUTBOUND_MESSAGE_CHANGE_FOCUS = 50,
    ACC_OUTBOUND_MESSAGE_INSTANT_REPLAY_REQUEST = 51,
    ACC_OUTBOUND_MESSAGE_PLAY_MANUAL_REPLAY_HIGHLIGHT = 52,
    ACC_OUTBOUND_MESSAGE_SAVE_MANUAL_REPLAY_HIGHLIGHT = 60,
} acc_outbound_message_type_t;

typedef enum {
    ACC_INBOUND_MESSAGE_REGISTRATION_RESULT = 1,
    ACC_INBOUND_MESSAGE_REAL_TIME_UPDATE = 2,
    ACC_INBOUND_MESSAGE_REAL_TIME_CAR_UPDATE = 3,
    ACC_INBOUND_MESSAGE_ENTRY_LIST = 4,
    ACC_INBOUND_MESSAGE_ENTRY_LIST_CAR = 6,
    ACC_INBOUND_MESSAGE_TRACK_DATA = 5,
    ACC_INBOUND_MESSAGE_BROADCASTING_EVENT = 7,
} acc_inbound_message_type_t;

typedef struct {
    string_t *first_name; // Driver's First Name
    string_t *last_name;  // Driver's Last Name
    string_t *short_name; // Short Driver Name
    uint8_t category;     // DriverCategory Enum
    uint16_t nationality; // Nationality Enum
} acc_driver_info_t;

typedef struct {
    uint32_t size;
    acc_driver_info_t *data;
} acc_driver_info_array_t;

typedef struct {
    uint16_t car_index;                    // Index from a Cars array
    uint8_t car_model_type;                // Model type of the car
    uint8_t cup_category;                  // Cup category
    string_t *team_name;                   // Team name of the car
    int32_t nationality;                   // Nationality Enum
    int32_t race_number;                   // Car number
    uint8_t current_driver_index;          // Index from a Drivers array
    acc_driver_info_array_t *drivers_info; // A Drivers array
} acc_car_info_t;

typedef struct {
    int32_t lap_time;       // Lap time (in ms)
    int32_array_t *splits;  // Lap split times
    uint16_t car_index;     // Index from a Cars array
    uint16_t driver_index;  // Index from a Drivers array
    bool is_invalid;        // Flag if a lap is invalid
    bool is_valid_for_best; // Flag if a lap can be considered the best attempt
    int32_t lap_type;       // LapType Enum
} acc_lap_info_t;

typedef struct {
    int32_t connection_id;       // Connection ID
    string_t *track_name;        // Name of the track
    int32_t track_id;            // ID of the track
    int32_t track_length;        // Track length (in meters)
    string_array_t *camera_sets; // Available Camera Sets (array)
    string_array_t *cameras;     // Available Cameras (array). Format is "{camera_sets_index}:{camera_name}"
    string_array_t *hud_pages;   // Available HUD Pages (array)
} acc_track_data_t;

typedef struct {
    uint8_t type;      // Broadcasting Event Type
    string_t *message; // Broadcasting Event Message
    int32_t time;      // Broadcasting Event Time (in ms)
    int32_t car_index; // Index from a Cars array
} acc_broadcasting_event_t;

typedef struct {
    uint16_t car_index;              // Index from a Cars array
    uint16_t driver_index;           // Index from a Drivers array
    uint8_t gear;                    // Gear the car is currently in
    float world_position_x;          // X Position on the track
    float world_position_y;          // Y Position on the track
    float yaw;                       // Yaw of the car
    uint8_t car_location;            // CarLocation Enum
    uint16_t speed;                  // Car's speed (in km/h)
    uint16_t position;               // Car's position
    uint16_t track_position;         // Car's track position
    float spline_position;           // Car's spline position
    int32_t delta;                   // Car's current delta
    acc_lap_info_t best_session_lap; // Best lap info
    acc_lap_info_t last_lap;         // Last lap info
    acc_lap_info_t current_lap;      // Current lap info
    uint16_t laps;                   // Laps
    uint16_t cup_position;           // Cup position
    uint8_t driver_count;            // Number of drivers from this car
} acc_rt_car_update_t;

typedef struct {
    uint16_t event_index;            // Index from a Events array
    uint16_t session_index;          // Index from a Session array
    uint8_t phase;                   // SessionPhase enum
    float session_time;              // Session Time (timespan)
    float remaining_time;            // Remaining Time (timespan)
    float time_of_day;               // Time of the day (timespan)
    uint8_t rain_level;              // Rain level
    uint8_t cloud_level;             // Cloud level
    uint8_t wetness_level;           // Wetness level
    acc_lap_info_t best_session_lap; // Best Session Lap
    uint16_t best_lap_car_index;     // Index from a Cars array
    uint16_t best_lap_driver_index;  // Index from a Drivers array
    int32_t focused_car_index;       // Index from a Cameras array
    string_t *active_camera_set;     // Name of the active camera set
    string_t *active_camera;         // Name of the active camera
    bool is_replay_playing;          // Flag of replay is playing
    float replay_session_time;       // Replay session time
    float replay_remaining_time;     // Replay remaining time
    float session_remaining_time;    // Session remaining time (timespan)
    float session_end_time;          // Session end time (timespan)
    uint8_t session_type;            // SessionType enum
    uint8_t ambient_temperature;     // Ambient temperature
    uint8_t track_temperature;       // Track temperature
    string_t *current_hud_page;      // Name of the current HUD page
} acc_rt_update_t;

typedef struct {
    int32_t connection_id;
    bool connection_success;
    bool is_read_only;
    string_t *error_message;
} acc_reg_result_t;

typedef struct {
    int32_t connection_id;
    uint16_array_t *indexes;
} acc_entry_list_t;

typedef struct {
    int32_t update_interval;
    string_t *display_name;
    string_t *connection_password;
    string_t *command_password;
} acc_reg_app_t;

typedef struct {
    uint8_t unused;
} acc_unreg_app_t;

typedef struct {
    int32_t connection_id;
} acc_req_entry_list_t;

typedef struct {
    int32_t connection_id;
} acc_req_track_data_t;

typedef struct {
    int32_t connection_id;
    string_t *hud_page;
} acc_change_hud_page_t;

typedef struct {
    int32_t connection_id;
    uint16_t *car_index;
    string_t *camera_set;
    string_t *camera;
} acc_change_focus_t;

typedef struct {
    int32_t connection_id;
    float start_session_time;
    float duration; // In ms
    int32_t initial_focused_car_index;
    string_t *initial_camera_set;
    string_t *initial_camera;
} acc_req_instant_replay_t;

typedef union {
    acc_reg_app_t *register_application;
    acc_unreg_app_t *unregister_application;
    acc_req_entry_list_t *request_entry_list;
    acc_req_track_data_t *request_track_data;
    acc_change_hud_page_t *request_hud_page;
    acc_change_focus_t *change_focus;
    acc_req_instant_replay_t *request_instant_replay;
} acc_server_request_data_t;

typedef union {
    acc_reg_result_t *registration_result;
    acc_rt_update_t *real_time_update;
    acc_rt_car_update_t *real_time_car_update;
    acc_entry_list_t *entry_list;
    acc_car_info_t *entry_list_car;
    acc_track_data_t *track_data;
    acc_broadcasting_event_t *broadcasting_event;
} acc_server_response_data_t;

typedef struct {
    acc_outbound_message_type_t type;
    acc_server_request_data_t data;
} acc_server_request_t;

typedef struct {
    acc_inbound_message_type_t type;
    acc_server_response_data_t data;
} acc_server_response_t;

typedef struct ACClient acc_client_t;

acc_client_t *acc_client_create(char *address, uint16_t port);
bool acc_client_destroy(acc_client_t *client);
int32_t acc_get_last_error(void);

bool acc_client_send(acc_client_t *client, acc_server_request_t *request);
bool acc_client_receive(acc_client_t *client, acc_server_response_t *response);
