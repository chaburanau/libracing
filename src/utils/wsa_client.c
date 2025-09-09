#include <WS2tcpip.h>
#include <WinSock2.h>
#include <stdint.h>
#include <winerror.h>

#pragma comment(lib, "Ws2_32.lib")

static WSADATA wsa_client_data;
static int wsa_client_users_counter = 0;

static int32_t wsa_client_add_user(void) {
    if (wsa_client_users_counter < 0) {
        return -1;
    }

    if (wsa_client_users_counter == 0) {
        const int32_t status = WSAStartup(MAKEWORD(2, 2), &wsa_client_data);
        if (status != 0) {
            return status;
        }
    }

    wsa_client_users_counter++;
    return 0;
}

static int32_t wsa_client_remove_user(void) {
    if (wsa_client_users_counter < 0) {
        return -1;
    }
    if (wsa_client_users_counter == 0) {
        return 0;
    }

    const int32_t status = WSACleanup();
    if (status != 0) {
        return WSAGetLastError();
    }

    wsa_client_users_counter--;
    return 0;
}
