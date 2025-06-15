#pragma once

#include <stdbool.h>
#include <winsock2.h>

#define AC_BUFFER_SIZE 408

static const int AC_IDENTIFIER = 1;
static const int AC_SERVER_VERSION = 1;
static const int AC_SERVER_PORT = 9996;
static const char* AC_SERVER_IP = "127.0.0.1";

typedef enum Status {
	AC_STATUS_OK = 0,
	AC_STATUS_NOT_INITIALIZED = 1,
	AC_STATUS_NOT_HANDSHAKED = 2,
	AC_STATUS_ALREADY_INITIALIZED = 3,
	AC_STATUS_ALREADY_HANDSHAKED = 4,
	AC_STATUS_CALLBACK_DEFINED = 5,
	AC_STATUS_SOCKET_ERROR = 6,
	AC_STATUS_SEND_ERROR = 7,
	AC_STATUS_RECEIVE_ERROR = 8,
	AC_STATUS_TIMEOUT_ERROR = 9,
	AC_STATUS_RECEIVED_INVALID_DATA = 10,
	AC_STATUS_NOT_DISMISSED = 11,
} ac_status_t;

typedef enum Operation {
	AC_OPERATION_HANDSHAKE = 0,
	AC_OPERATION_SUBSCRIBE_UPDATE = 1,
	AC_OPERATION_SUBSCRIBE_SPOT = 2,
	AC_OPERATION_DISMISS = 3,
} ac_operation_t;

#pragma pack(1)
typedef struct Request {
	int identifier;				// Client's device identifier. See `AC_IDENTIFIER`
	int version;				// Version of the API. See `AC_SERVER_VERSION`
	ac_operation_t operation;	// Type of the operation the client wants to perform. See `ac_operation_t`
} ac_request_t;

#pragma pack(1)
typedef struct Response {
	wchar_t car_name[50];		// Name of the car that player is driving
	wchar_t driver_name[50];	// Name of the driver playing
	int identifier;				// For now, it is just 4242; this code will identify different status
	int version;				// For now, it is just 1; this code will identify the version server is running
	wchar_t track_name[50];		// Name of the track that is running
	wchar_t track_config[50];	// Name of the track configuration that is running
} ac_response_t;

#pragma pack(1)
typedef struct RealTimeCarInfo {
	char identifier[4];
	int size;

	float speed_kmh;	// Speed in km/h
	float speed_mph;	// Speed in mp/h
	float speed_ms;		// Speed in m/s

	bool is_abs_enabled;		// Is ABS enabled
	bool is_abs_in_action;		// Is ABS in action
	bool is_tc_in_action;		// IS TC in action
	bool is_tc_enabled;			// Is TC enabled
	bool is_in_pit;				// Is in pit
	bool is_engine_limiter_on;	// Is Engine limiter on

	bool _unknown_1; // Unknown byte 1
	bool _unknown_2; // Unknown byte 2

	float acc_g_vertical;		// Acceleration Vertical
	float acc_g_horizontal;		// Acceleration Horizonal
	float acc_g_frontal;		// Acceleration Frontal

	int lap_time;	// Lap time
	int last_lap;	// Last lap time
	int best_lap;	// Best lap time
	int lap_count;	// Lap count

	float gas;			// Gas state
	float brake;		// Brake state
	float clutch;		// Clutch state
	float engine_rpm;	// Engine RPM
	float steer;		// Steering angle
	int gear;			// Gear
	float cg_height;	// ????

	float wheel_angular_speed[4];
	float slip_angle[4];
	float slip_angle_contact_patch[4];
	float slip_ratio[4];
	float tyre_slip[4];
	float nd_slip[4];
	float load[4];
	float dy[4];
	float mz[4];
	float tyre_dirty_level[4];

	float camber_radius[4];
	float tyre_radius[4];
	float tyre_loaded_radius[4];
	float suspension_height[4];

	float car_position_normalized;
	float car_slope;
	float car_coordinates[3];
} ac_rt_car_info;

#pragma pack(1)
typedef struct RealTimeLapInfo {
	int car_number;				// Car number
	int lap;					// Lap the car is on
	wchar_t driver_name[50];	// Driver name of the car
	wchar_t car_name[50];		// Car name
	int time;					// Total Time
} ac_rt_lap_info;

typedef enum EventType {
	AC_EVENT_TYPE_HANDSHAKE = 1,
	AC_EVENT_TYPE_CAR_INFO = 2,
	AC_EVENT_TYPE_LAP_INFO = 3,
} ac_event_type_t;

typedef union Event {
	ac_response_t *handshake;
	ac_rt_car_info *car_info;
	ac_rt_lap_info *lap_info;
} ac_event_t;

typedef struct Client {
	bool _socket_initialized;
	bool _address_initialized;
	bool _handshake_initialized;

	SOCKET _socket;
	struct sockaddr_in _server_address;
} ac_client_t;

ac_client_t ac_new_client();
ac_status_t ac_client_init(ac_client_t *client);
ac_status_t ac_client_close(ac_client_t *client);
ac_status_t ac_client_handshake(ac_client_t *client);
ac_status_t ac_client_subscribe_update(ac_client_t *client);
ac_status_t ac_client_subscribe_spot(ac_client_t *client);
ac_status_t ac_client_dismiss(ac_client_t *client);
ac_status_t ac_client_receive(ac_client_t *client, char buffer[AC_BUFFER_SIZE], ac_event_t *event, ac_event_type_t *event_type);