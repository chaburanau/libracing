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
    char first_name[100];
    char last_name[100];
    char short_name[10];
    int category;
    char nationality[50];
} acc_driver_info_t;

typedef struct CarInfo {
    unsigned short car_index;
    char car_model_type;
    char team_name[100];
    int race_number;
    char cup_category;
    int current_driver_index;
    acc_driver_info_t driver_info[4];
    char nationality[50];
} acc_car_info_t;

typedef struct LapInfo {
    int lap_time;
    int splits[];
    unsigned short car_index;
    unsigned short driver_index;
    bool is_invalid;
    bool is_valid_for_best;
    int lap_type;
} acc_lap_info_t;

typedef struct TrackData {
    char track_name[100];
    int track_id;
    float track_length;
    char camera_sets[100];
    char hud_pages[100];
} acc_track_data_t;

typedef struct BroadcastingEvent {
    int type;
    char message[256];
    int time;
    int car_id;
    acc_car_info_t car_info;
} acc_broadcasting_event_t;

typedef struct RealTimeCarUpdate {
    int car_index;
    int driver_index;
    int gear;
    float world_position_x;
    float world_position_y;
    float yaw;
    int car_location; // Enum
    int speed;
    int position;
    int track_position;
    float spline_position;
    int delta;
    acc_lap_info_t best_session_lap;
    acc_lap_info_t last_lap;
    acc_lap_info_t current_lap;
    int laps;
    unsigned short cup_position;
    char driver_count;
} acc_rt_car_update_t;

typedef struct RealTimeUpdate {
    int event_index;
    int session_index;
    int phase; // SessionPhase enum
    int session_time;
    int remaining_time;
    int time_of_day;
    float rain_level;
    float cloud_level;
    float wetness_level;
    acc_lap_info_t best_session_lap;
    unsigned short best_lap_car_index;
    unsigned short best_lap_driver_index;
    int focused_car_index;
    char active_camera_set[100];
    char active_camera[100];
    bool is_replay_playing;
    float replay_session_time;
    float replay_remaining_time;
    float session_remaining_time;
    float session_end_time;
    int session_type; // SessionType enum
    char ambient_temperature;
    char track_temperature;
    char current_hud_page[100];
} acc_rt_update_t;