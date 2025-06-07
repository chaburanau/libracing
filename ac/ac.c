#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ac.h"

#pragma comment(lib, "Ws2_32.lib")

#define HANDSHAKE_COUNT 3      // Number of handshake messages to send
#define BUFFER_SIZE 1024       // Receive buffer size
#define TIMEOUT_MILLISECONDS 2000  // Timeout for recv in ms


bool ac_handshake() {
	return false;
}

bool ac_subscribe_update(void (*callback)(ac_rt_car_info)) {
	return false;
}

bool ac_subscribe_spot(void (*callback)(ac_rt_lap_info)) {
	return false;
}

bool ac_dismiss() {
	return false;
}
