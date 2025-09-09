#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <winerror.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include "./wsa_client.c"

#pragma comment(lib, "Ws2_32.lib")

static int32_t udp_socket_address_size = sizeof(SOCKADDR_IN);
static int32_t udp_socket_last_error = 0;

typedef struct {
  SOCKADDR_IN _address;
  SOCKET _socket;
  bool _wsa_initialized;
} udp_socket_t;

udp_socket_t *udp_socket_create(const char *address, uint16_t port);
bool udp_socket_destroy(udp_socket_t *udp_socket);
int32_t udp_socket_send(udp_socket_t *udp_socket, char *data, size_t size);
int32_t udp_socket_receive(udp_socket_t *udp_socket, char *data, size_t size);
int32_t udp_socket_get_last_error(void);

udp_socket_t *udp_socket_create(const char *address, uint16_t port) {
  udp_socket_t *udp_socket = malloc(sizeof(udp_socket_t));
  if (udp_socket == NULL) {
    udp_socket_last_error = -1;
    return NULL;
  }

  ZeroMemory(&udp_socket->_address, udp_socket_address_size);
  udp_socket->_address.sin_family = AF_INET;
  udp_socket->_address.sin_port = htons(port);
  udp_socket->_socket = INVALID_SOCKET;
  udp_socket->_wsa_initialized = false;

  const int32_t wsa_status = wsa_client_add_user();
  if (wsa_status != 0) {
    udp_socket_last_error = wsa_status;
    udp_socket_destroy(udp_socket);
    return NULL;
  }

  udp_socket->_wsa_initialized = true;

  udp_socket->_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (udp_socket->_socket == INVALID_SOCKET) {
    udp_socket_last_error = WSAGetLastError();
    udp_socket_destroy(udp_socket);
    return NULL;
  }

  const int32_t inet_pton_status = inet_pton(AF_INET, address, &udp_socket->_address.sin_addr);
  if (inet_pton_status != 1) {
    udp_socket_last_error = WSAGetLastError();
    udp_socket_destroy(udp_socket);
    return NULL;
  }

  return udp_socket;
}

bool udp_socket_destroy(udp_socket_t *udp_socket) {
  if (udp_socket == NULL) {
    return false;
  }

  if (udp_socket->_socket != INVALID_SOCKET) {
    const int32_t status = closesocket(udp_socket->_socket);
    if (status != 0) {
      udp_socket_last_error = WSAGetLastError();
      return true;
    }
  }

  if (udp_socket->_wsa_initialized == true) {
    const int32_t wsa_status = wsa_client_remove_user();
    if (wsa_status != 0) {
      udp_socket_last_error = wsa_status;
      return true;
    }
  }

  free(udp_socket);
  return false;
}

struct sockaddr* _address(udp_socket_t* udp_socket) {
  return (struct sockaddr *)&udp_socket->_address;
}

int32_t udp_socket_send(udp_socket_t *udp_socket, char *data, size_t size) {
  if (udp_socket == NULL) {
    udp_socket_last_error = -1000;
    return -1000;
  }

  const int32_t bytes_sent = sendto(udp_socket->_socket, data, (int32_t)size, 0, _address(udp_socket), udp_socket_address_size);
  if (bytes_sent == SOCKET_ERROR) {
    udp_socket_last_error = WSAGetLastError();
    return -1;
  }

  return bytes_sent;
}

int32_t udp_socket_receive(udp_socket_t *udp_socket, char *data, size_t size) {
  if (udp_socket == NULL) {
    udp_socket_last_error = -1;
    return -1;
  }

  const int32_t bytes_received = recvfrom(udp_socket->_socket, data, (int32_t)size, 0, _address(udp_socket), &udp_socket_address_size);
  if (bytes_received == SOCKET_ERROR) {
    udp_socket_last_error = WSAGetLastError();
    return -1;
  }

  return bytes_received;
}

int32_t udp_socket_get_last_error(void) {
  return udp_socket_last_error;
}
