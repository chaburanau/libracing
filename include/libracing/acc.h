#pragma once

#include <stdbool.h>

const int ACC_BROADCASTING_PROTOCOL_VERSION = 4;

typedef enum LapType {
    ACC_LAP_TYPE_INVALID = 0,
    ACC_LAP_TYPE_OUTLAP = 1,
    ACC_LAP_TYPE_REGULAR = 2,
    ACC_LAP_TYPE_INLAP = 3,
} acc_lap_type_t;

typedef enum DriverCategory {
    ACC_DRIVER_CATEGORY_BRONZE = 0,
    ACC_DRIVER_CATEGORY_SILVER = 1,
    ACC_DRIVER_CATEGORY_GOLD = 2,
    ACC_DRIVER_CATEGORY_PLATINUM = 3,
    ACC_DRIVER_CATEGORY_UNKNOWN = 255,
} acc_driver_category_t;

typedef enum CarLocation {
    ACC_CAR_LOCATION_UNKNOWN = 0,
    ACC_CAR_LOCATION_TRACK = 1,
    ACC_CAR_LOCATION_PITLANE = 2,
    ACC_CAR_LOCATION_PITENTRY = 3,
    ACC_CAR_LOCATION_PITEXIT = 4,
} acc_car_location_t;

typedef enum SessionPhase {
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

typedef enum SessionType {
    ACC_SESSION_TYPE_PRACTICE = 0,
    ACC_SESSION_TYPE_QUALIFYING = 4,
    ACC_SESSION_TYPE_SUPERPOLE = 9,
    ACC_SESSION_TYPE_RACE = 10,
    ACC_SESSION_TYPE_HOT_LAP = 11,
    ACC_SESSION_TYPE_HOT_STINT = 12,
    ACC_SESSION_TYPE_HOT_LAP_SUPERPOLE = 13,
    ACC_SESSION_TYPE_REPLAY = 14,
} acc_session_type_t;

typedef enum BroadcastingEventType {
    ACC_BROADCASTING_EVENT_TYPE_UNKNOWN = 0,
    ACC_BROADCASTING_EVENT_TYPE_GREEN_FLAG = 1,
    ACC_BROADCASTING_EVENT_TYPE_SESSION_OVER = 2,
    ACC_BROADCASTING_EVENT_TYPE_PENALTY_COMMUNICATION_MESSAGE = 3,
    ACC_BROADCASTING_EVENT_TYPE_ACCIDENT = 4,
    ACC_BROADCASTING_EVENT_TYPE_LAP_COMPLETED = 5,
    ACC_BROADCASTING_EVENT_TYPE_BEST_SESSION_LAP = 6,
    ACC_BROADCASTING_EVENT_TYPE_BEST_PERSONAL_LAP = 7,
} acc_broadcasting_event_type_t;

typedef enum Nationality {
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

typedef enum OutboundMessageTypes {
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

typedef enum InboundMessageTypes {
    ACC_INBOUND_MESSAGE_REGISTRATION_RESULT = 1,
    ACC_INBOUND_MESSAGE_REAL_TIME_UPDATE = 2,
    ACC_INBOUND_MESSAGE_REAL_TIME_CAR_UPDATE = 3,
    ACC_INBOUND_MESSAGE_ENTRY_LIST = 4,
    ACC_INBOUND_MESSAGE_ENTRY_LIST_CAR = 6,
    ACC_INBOUND_MESSAGE_TRACK_DATA = 5,
    ACC_INBOUND_MESSAGE_BROADCASTING_EVENT = 7,
} acc_inbound_message_type_t;

typedef struct DriverInfo {
    char *first_name;  // Driver's First Name
    char *last_name;   // Driver's Last Name
    char *short_name;  // Short Driver Name
    int category;      // DriverCategory Enum
    int nationality;   // Nationality Enum
} acc_driver_info_t;

typedef struct CarInfo {
    unsigned short car_index;         // Index from a Cars array
    char car_model_type;              // Model type of the car
    char cup_category;                // Cup category
    char *team_name;                  // Team name of the car
    int nationality;                  // Nationality Enum
    int race_number;                  // Car number
    int current_driver_index;         // Index from a Drivers array
    acc_driver_info_t driver_info[4]; // A Drivers array
} acc_car_info_t;

typedef struct LapInfo {
    int lap_time;                // Lap time (in ms)
    int splits[];                // Lap split times
    unsigned short car_index;    // Index from a Cars array
    unsigned short driver_index; // Index from a Drivers array
    bool is_invalid;             // Flag if a lap is invalid
    bool is_valid_for_best;      // Flag if a lap can be counted towards the best attempt
    int lap_type;                // LapType Enum
} acc_lap_info_t;

typedef struct TrackData {
    char *track_name;      // Name of the track
    int track_id;          // ID of the track
    float track_length;    // Track length (in meters)
    char **camera_sets;    // Available Camera Sets (dictionary)
    char **hud_pages;      // Available HUD Pages (array)
} acc_track_data_t;

typedef struct BroadcastingEvent {
    int type;                // Broadcasting Event Type
    char *message;           // Broadcasting Event Message
    int time;                // Broadcasting Event Time (in ms)
    int car_id;              // Identifier of a car
    acc_car_info_t car_info; // Car info
} acc_broadcasting_event_t;

typedef struct RealTimeCarUpdate {
    int car_index;                   // Index from a Cars array
    int driver_index;                // Index from a Drivers array
    int gear;                        // Gear the car is currently in
    float world_position_x;          // X Position on the track
    float world_position_y;          // Y Position on the track
    float yaw;                       // Yaw of the car
    int car_location;                // CarLocation Enum
    int speed;                       // Car's speed (in km/h)
    int position;                    // Car's position
    int track_position;              // Car's track position
    float spline_position;           // Car's spline position
    int delta;                       // Car's current delta
    acc_lap_info_t best_session_lap; // Best lap info
    acc_lap_info_t last_lap;         // Last lap info
    acc_lap_info_t current_lap;      // Current lap info
    int laps;                        // Laps
    unsigned short cup_position;     // Cup position
    char driver_count;               // Number of drivers from this car
} acc_rt_car_update_t;

typedef struct RealTimeUpdate {
    int event_index;                      // Index from a Events array
    int session_index;                    // Index from a Session array
    int phase;                            // SessionPhase enum
    int session_time;                     // Session Time (timespan)
    int remaining_time;                   // Remaining Time (timespan)
    int time_of_day;                      // Time of the day (timespan)
    float rain_level;                     // Rain level
    float cloud_level;                    // Cloud level
    float wetness_level;                  // Wetness level
    acc_lap_info_t best_session_lap;      // Best Session Lap
    unsigned short best_lap_car_index;    // Index from a Cars array
    unsigned short best_lap_driver_index; // Index from a Drivers array
    int focused_car_index;                // Index from a Cameras array
    char *active_camera_set;              // Name of the active camera set
    char *active_camera;                  // Name of the active camera
    bool is_replay_playing;               // Flag of replay is playing
    float replay_session_time;            // Replay session time
    float replay_remaining_time;          // Replay remaining time
    float session_remaining_time;         // Session remaining time (timespan)
    float session_end_time;               // Session end time (timespan)
    int session_type;                     // SessionType enum
    char ambient_temperature;             // Ambient temperature
    char track_temperature;               // Track temperature
    char *current_hud_page;               // Name of the current HUD page
} acc_rt_update_t;