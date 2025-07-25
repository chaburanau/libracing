#pragma once

#include <stdint.h>
#include <stdbool.h>

#define AC_IDENTIFIER 1
#define AC_SERVER_VERSION 1
#define AC_SERVER_PORT 9996
#define AC_SERVER_IP "127.0.0.1"

typedef enum {
	AC_STATUS_OK = 0,
	AC_STATUS_CLIENT_NOT_INITIALIZED = 100,
	AC_STATUS_HANDSHAKE_NOT_PERFORMED = 101,
	AC_STATUS_ALLOCATION_FAILED = 102,
	AC_STATUS_SOCKET_ERROR = 200,
	AC_STATUS_SEND_ERROR = 300,
	AC_STATUS_RECEIVE_ERROR = 400,
	AC_STATUS_RECEIVED_INVALID_DATA = 401,
} ac_status_t;

typedef enum {
	AC_OPERATION_HANDSHAKE = 0,
	AC_OPERATION_SUBSCRIBE_UPDATE = 1,
	AC_OPERATION_SUBSCRIBE_SPOT = 2,
	AC_OPERATION_DISMISS = 3,
} ac_operation_t;

typedef struct {
	int32_t identifier;			// Client's device identifier. See `AC_IDENTIFIER`
	int32_t version;			// Version of the API. See `AC_SERVER_VERSION`
	ac_operation_t operation;	// Type of the operation the client wants to perform. See `ac_operation_t`
} ac_request_t;

typedef struct {
	wchar_t car_name[50];		// Name of the car that player is driving
	wchar_t driver_name[50];	// Name of the driver playing
	int32_t identifier;			// For now, it is just 4242; this code will identify different status
	int32_t version;			// For now, it is just 1; this code will identify the version the server is running
	wchar_t track_name[50];		// Name of the track that is running
	wchar_t track_config[50];	// Name of the track configuration that is running
} ac_response_t;

typedef struct {
	int8_t identifier[4];
	int32_t size;

	float speed_kmh;	// Speed in km/h
	float speed_mph;	// Speed in mp/h
	float speed_ms;		// Speed in m/s

	bool is_abs_enabled;		// Is ABS enabled
	bool is_abs_in_action;		// Is ABS in action
	bool is_tc_in_action;		// IS TC in action
	bool is_tc_enabled;			// Is TC enabled
	bool is_in_pit;				// Is in pit
	bool is_engine_limiter_on;	// Is Engine limiter on

	int8_t _unknown_1; // Unknown byte 1
	int8_t _unknown_2; // Unknown byte 2

	float acc_g_vertical;		// Acceleration Vertical
	float acc_g_horizontal;		// Acceleration Horizonal
	float acc_g_frontal;		// Acceleration Frontal

	int32_t lap_time;	// Lap time
	int32_t last_lap;	// Last lap time
	int32_t best_lap;	// Best lap time
	int32_t lap_count;	// Lap count

	float gas;			// Gas state
	float brake;		// Brake state
	float clutch;		// Clutch state
	float engine_rpm;	// Engine RPM
	float steer;		// Steering angle
	int32_t gear;		// Gear
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

typedef struct {
	int32_t car_number;			// Car number
	int32_t lap;				// Lap the car is on
	wchar_t driver_name[50];	// Driver name of the car
	wchar_t car_name[50];		// Car name
	int32_t time;				// Total Time
} ac_rt_lap_info;

typedef enum {
	AC_EVENT_TYPE_HANDSHAKE = 1,
	AC_EVENT_TYPE_CAR_INFO = 2,
	AC_EVENT_TYPE_LAP_INFO = 3,
} ac_event_type_t;

typedef union {
	ac_response_t *handshake;
	ac_rt_car_info *car_info;
	ac_rt_lap_info *lap_info;
} ac_event_data_t;

typedef struct {
	ac_event_type_t type;
	ac_event_data_t data;
} ac_event_t;

typedef struct AssettoCorsaClient ac_client_t;

ac_client_t *ac_client_create(const char *address, int port);
void ac_client_destroy(ac_client_t *client);

ac_status_t ac_client_handshake(ac_client_t *client);
ac_status_t ac_client_subscribe_update(ac_client_t *client);
ac_status_t ac_client_subscribe_spot(ac_client_t *client);
ac_status_t ac_client_dismiss(ac_client_t *client);
ac_status_t ac_client_receive(ac_client_t *client, ac_event_t *event);
