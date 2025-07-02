#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "udp_socket.h"

typedef struct {
    int length;
    char *data;
} string_t;

typedef struct {
    int length;
    int32_t *data;
} int32_array_t;

typedef struct {
    int length;
    uint16_t *data;
} uint16_array_t;

typedef struct {
    int length;
    string_t *data;
} string_array_t;

const int8_t ACC_BROADCASTING_PROTOCOL_VERSION = 4;
const int32_t ACC_SERVER_PORT = 9000;
const int32_t ACC_LOCAL_PORT = 9001;
const char *ACC_SERVER_IP = "127.0.0.1";
const char *ACC_LOCAL_IP = "127.0.0.1";
const char *ACC_CONNECTION_PASSWORD = "asd";
const char *ACC_COMMAND_PASSWORD = "";

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
    ACC_STATUS_BUFFER_SIZE_MISMATCH = 300,
    ACC_STATUS_UNEXPECTED_AMOUNT_OF_BYTES_SENT = 400,
    ACC_STATUS_UNEXPECTED_AMOUNT_OF_BYTES_RECEIVED = 500,
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
    Any = 0,
    Italy = 1,
    Germany = 2,
    France = 3,
    Spain = 4,
    GreatBritain = 5,
    Hungary = 6,
    Belgium = 7,
    Switzerland = 8,
    Austria = 9,
    Russia = 10,
    Thailand = 11,
    Netherlands = 12,
    Poland = 13,
    Argentina = 14,
    Monaco = 15,
    Ireland = 16,
    Brazil = 17,
    SouthAfrica = 18,
    PuertoRico = 19,
    Slovakia = 20,
    Oman = 21,
    Greece = 22,
    SaudiArabia = 23,
    Norway = 24,
    Turkey = 25,
    SouthKorea = 26,
    Lebanon = 27,
    Armenia = 28,
    Mexico = 29,
    Sweden = 30,
    Finland = 31,
    Denmark = 32,
    Croatia = 33,
    Canada = 34,
    China = 35,
    Portugal = 36,
    Singapore = 37,
    Indonesia = 38,
    USA = 39,
    NewZealand = 40,
    Australia = 41,
    SanMarino = 42,
    UAE = 43,
    Luxembourg = 44,
    Kuwait = 45,
    HongKong = 46,
    Colombia = 47,
    Japan = 48,
    Andorra = 49,
    Azerbaijan = 50,
    Bulgaria = 51,
    Cuba = 52,
    CzechRepublic = 53,
    Estonia = 54,
    Georgia = 55,
    India = 56,
    Israel = 57,
    Jamaica = 58,
    Latvia = 59,
    Lithuania = 60,
    Macau = 61,
    Malaysia = 62,
    Nepal = 63,
    NewCaledonia = 64,
    Nigeria = 65,
    NorthernIreland = 66,
    PapuaNewGuinea = 67,
    Philippines = 68,
    Qatar = 69,
    Romania = 70,
    Scotland = 71,
    Serbia = 72,
    Slovenia = 73,
    Taiwan = 74,
    Ukraine = 75,
    Venezuela = 76,
    Wales = 77,
    Iran = 78,
    Bahrain = 79,
    Zimbabwe = 80,
    ChineseTaipei = 81,
    Chile = 82,
    Uruguay = 83,
    Madagascar = 84,
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
    string_t *first_name;   // Driver's First Name
    string_t *last_name;    // Driver's Last Name
    string_t *short_name;   // Short Driver Name
    int32_t category;       // DriverCategory Enum
    int32_t nationality;    // Nationality Enum
} acc_driver_info_t;

typedef struct {
    uint16_t car_index;                 // Index from a Cars array
    int8_t car_model_type;              // Model type of the car
    int8_t cup_category;                // Cup category
    string_t *team_name;                // Team name of the car
    int32_t nationality;                // Nationality Enum
    int32_t race_number;                // Car number
    int32_t current_driver_index;       // Index from a Drivers array
    acc_driver_info_t driver_info[4];   // A Drivers array
} acc_car_info_t;

typedef struct {
    int32_t lap_time;           // Lap time (in ms)
    int32_array_t *splits[];    // Lap split times
    uint16_t car_index;         // Index from a Cars array
    uint16_t driver_index;      // Index from a Drivers array
    bool is_invalid;            // Flag if a lap is invalid
    bool is_valid_for_best;     // Flag if a lap can be counted towards the best attempt
    int32_t lap_type;           // LapType Enum
} acc_lap_info_t;

typedef struct {
    string_t *track_name;           // Name of the track
    int32_t track_id;               // ID of the track
    float track_length;             // Track length (in meters)
    string_array_t *camera_sets;    // Available Camera Sets (array)
    string_array_t *cameras;        // Available Cameras (array). Format is "{camera_sets_index}:{camera_name}"
    string_array_t *hud_pages;      // Available HUD Pages (array)
} acc_track_data_t;

typedef struct {
    int32_t type;                // Broadcasting Event Type
    string_t *message;           // Broadcasting Event Message
    int32_t time;                // Broadcasting Event Time (in ms)
    int32_t car_index;           // Index from a Cars array
} acc_broadcasting_event_t;

typedef struct {
    int32_t car_index;                  // Index from a Cars array
    int32_t driver_index;               // Index from a Drivers array
    int32_t gear;                       // Gear the car is currently in
    float world_position_x;             // X Position on the track
    float world_position_y;             // Y Position on the track
    float yaw;                          // Yaw of the car
    int32_t car_location;               // CarLocation Enum
    int32_t speed;                      // Car's speed (in km/h)
    int32_t position;                   // Car's position
    int32_t track_position;             // Car's track position
    float spline_position;              // Car's spline position
    int32_t delta;                      // Car's current delta
    acc_lap_info_t best_session_lap;    // Best lap info
    acc_lap_info_t last_lap;            // Last lap info
    acc_lap_info_t current_lap;         // Current lap info
    int32_t laps;                       // Laps
    uint16_t cup_position;              // Cup position
    int8_t driver_count;                // Number of drivers from this car
} acc_rt_car_update_t;

typedef struct {
    int32_t event_index;                // Index from a Events array
    int32_t session_index;              // Index from a Session array
    int32_t phase;                      // SessionPhase enum
    int32_t session_time;               // Session Time (timespan)
    int32_t remaining_time;             // Remaining Time (timespan)
    int32_t time_of_day;                // Time of the day (timespan)
    float rain_level;                   // Rain level
    float cloud_level;                  // Cloud level
    float wetness_level;                // Wetness level
    acc_lap_info_t best_session_lap;    // Best Session Lap
    uint16_t best_lap_car_index;        // Index from a Cars array
    uint16_t best_lap_driver_index;     // Index from a Drivers array
    int32_t focused_car_index;          // Index from a Cameras array
    int8_t *active_camera_set;          // Name of the active camera set
    int8_t *active_camera;              // Name of the active camera
    bool is_replay_playing;             // Flag of replay is playing
    float replay_session_time;          // Replay session time
    float replay_remaining_time;        // Replay remaining time
    float session_remaining_time;       // Session remaining time (timespan)
    float session_end_time;             // Session end time (timespan)
    int32_t session_type;               // SessionType enum
    int8_t ambient_temperature;         // Ambient temperature
    int8_t track_temperature;           // Track temperature
    int8_t *current_hud_page;           // Name of the current HUD page
} acc_rt_update_t;

typedef struct {
    int32_t connection_id;
    bool connection_success;
    bool is_read_only;
    string_t *error_message;
} acc_reg_result_t;

typedef struct {
    uint16_array_t *indexes;
} acc_entry_list_t;

typedef struct {
    int32_t update_interval;
    string_t *display_name;
    string_t *connection_password;
    string_t *command_password;
} acc_reg_app_t;

typedef struct {
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
    acc_reg_app_t            *register_application;
    acc_unreg_app_t          *unregister_application;
    acc_req_entry_list_t     *request_entry_list;
    acc_req_track_data_t     *request_track_data;
    acc_change_hud_page_t    *request_hud_page;
    acc_change_focus_t       *change_focus;
    acc_req_instant_replay_t *request_instant_replay;
} acc_server_request_t;

typedef union {
    acc_reg_result_t         *registration_result;
    acc_rt_update_t          *real_time_update;
    acc_rt_car_update_t      *real_time_car_update;
    acc_entry_list_t         *entry_list;
    acc_car_info_t           *entry_list_car;
    acc_track_data_t         *track_data;
    acc_broadcasting_event_t *broadcasting_event;
} acc_server_response_t;

typedef struct {
    int connection_id;
    udp_socket_t *socket;
} acc_client_t;

acc_client_t *acc_client_create(const char *address, int port);
void acc_client_close(acc_client_t *client);

acc_status_t acc_client_send(acc_client_t *client, acc_outbound_message_type_t type, acc_server_request_t request);
acc_status_t acc_client_receive(acc_client_t *client, acc_inbound_message_type_t *type, acc_server_response_t *response);