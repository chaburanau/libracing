#pragma once

#include <stdbool.h>

static const int AC_IDENTIFIER = 1;
static const int AC_SERVER_VERSION = 1;
static const int AC_SERVER_PORT = 9996;
static const char* AC_SERVER_IP = "127.0.0.1";

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
	char car_name[50];		// Name of the car that player is driving
	char driver_name[50];	// Name of the driver playing
	int identifier;			// For now it is just 4242, this code will identify different status
	int version;			// For now it is just 1, this code will idenitfy the version server is running
	char track_name[50];	// Name of the track that is running
	char track_config[50];	// Name of the track configuration that is running
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
	char driverName[50];	// Driver name of the car
	char carName[50];		// Car name
	int time;				// Total Time ???
} ac_rt_lap_info;

bool ac_init();
bool ac_handshake();
bool ac_subscribe_update(void (*callback)(ac_rt_car_info));
bool ac_subscribe_spot(void (*callback)(ac_rt_lap_info));
bool ac_dismiss();
bool ac_close();
