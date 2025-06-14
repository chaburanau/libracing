#pragma once

#include <stdbool.h>

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
} ac_status_t;

typedef enum Operation {
	AC_OPERATION_HANDSHAKE = 0,
	AC_OPERATION_SUBSCRIBE_UPDATE = 1,
	AC_OPERATION_SUBSCRIBE_SPOT = 2,
	AC_OPERATION_DISMISS = 3,
} ac_operation_t;

typedef struct HandshakerRequest {
	int identifier;		// Client's device identifier. See `AC_IDENTIFIER`
	int version;		// Version of the API. See `AC_SERVER_VERSION`
	int operation;		// Type of the operation client wants to perform. See `ac_operation_t`
} ac_handshaker_request_t;

typedef struct HandshakerResponse {
	wchar_t car_name[50];		// Name of the car that player is driving
	wchar_t driver_name[50];	// Name of the driver playing
	int identifier;				// For now, it is just 4242, this code will identify different status
	int version;				// For now, it is just 1, this code will identify the version server is running
	wchar_t track_name[50];		// Name of the track that is running
	wchar_t track_config[50];	// Name of the track configuration that is running
} ac_handshaker_response_t;

typedef struct RealTimeCarInfo {
	char identifier;
	int size;

	float speed_Kmh;	// Speed in km/h
	float speed_Mph;	// Speed in mp/h
	float speed_Ms;		// Speed in m/s

	bool isAbsEnabled;			// Is ABS enabled
	bool isAbsInAction;			// Is ABS in action
	bool isTcInAction;			// IS TC in action
	bool isTcEnabled;			// Is TC enabled
	bool isInPit;				// Is in pit
	bool isEngineLimiterOn;		// Is Engine limiter on


	float accG_vertical;	// Acceleration Vertical
	float accG_horizontal;	// Acceleration Horizonal
	float accG_frontal;		// Acceleration Frontal

	int lapTime;	// Lap time
	int lastLap;	// Last lap time
	int bestLap;	// Best lap time
	int lapCount;	// Lap count

	float gas;			// Gas state
	float brake;		// Brake state
	float clutch;		// Clutch state
	float engineRPM;	// Engine RPM
	float steer;		// Steering angle
	int gear;			// Gear
	float cgHeight;		// ????

	float wheelAngularSpeed[4];
	float slipAngle[4];
	float slipAngle_ContactPatch[4];
	float slipRatio[4];
	float tyreSlip[4];
	float ndSlip[4];
	float load[4];
	float Dy[4];
	float Mz[4];
	float tyreDirtyLevel[4];

	float camberRAD[4];
	float tyreRadius[4];
	float tyreLoadedRadius[4];
	float suspensionHeight[4];

	float carPositionNormalized;
	float carSlope;
	float carCoordinates[3];
} ac_rt_car_info;

typedef struct RealTimeLapInfo {
	int carNumber;			// Car number
	int lap;				// Lap the car is on
	wchar_t driverName[50];	// Driver name of the car
	wchar_t carName[50];	// Car name
	int time;				// Total Time ???
} ac_rt_lap_info;

typedef enum EventType {
	AC_EVENT_TYPE_HANDSHAKE = 1,
	AC_EVENT_TYPE_CAR_INFO = 2,
	AC_EVENT_TYPE_LAP_INFO = 3,
} ac_event_type_t;

typedef union Event {
	ac_handshaker_response_t *handshake;
	ac_rt_car_info *car_info;
	ac_rt_lap_info *lap_info;
} ac_event_t;

ac_status_t ac_init();	// ac_init inits udp socket
ac_status_t ac_close();	// ac_close closes udp socket

ac_status_t ac_send(ac_handshaker_request_t request);														// ac_send_handshake sends handshake message
ac_status_t ac_receive(char *buffer, int buffer_size, ac_event_t *event, ac_event_type_t *event_type);		// ac_receive_event returns event from the socket