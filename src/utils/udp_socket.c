#include <stdlib.h>
#include <stdint.h>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <winerror.h>

#pragma comment(lib, "Ws2_32.lib")

int udp_socket_address_size = sizeof(SOCKADDR_IN);

static WSADATA udp_socket_wsa_data;
static int udp_socket_wsa_users = 0;

typedef struct UDPSocket {
   SOCKET      _socket;
   SOCKADDR_IN _address;
} udp_socket_t;

udp_socket_t *udp_socket_create(const char* address, int port) {
   udp_socket_t *udp_socket = malloc(sizeof(udp_socket_t));
   if (udp_socket == NULL) return NULL;

   if (udp_socket_wsa_users == 0) {
      const int wsa_startup_status = WSAStartup(MAKEWORD(2, 2), &udp_socket_wsa_data);
      if (wsa_startup_status != 0) {
         free(udp_socket);
         return NULL;
      }
   }

   udp_socket_wsa_users++;

   udp_socket->_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (udp_socket->_socket == INVALID_SOCKET) {
      free(udp_socket);
      return NULL;
   }

   ZeroMemory(&udp_socket->_address, udp_socket_address_size);
   udp_socket->_address.sin_family = AF_INET;
   udp_socket->_address.sin_port = htons(port);

   const int32_t inet_pton_status = inet_pton(AF_INET, address, &udp_socket->_address.sin_addr);
   if (inet_pton_status != 1) {
      free(udp_socket);
      return NULL;
   }

   return udp_socket;
}

void udp_socket_destroy(udp_socket_t *udp_socket) {
   if (udp_socket == NULL) return;
   if (udp_socket->_socket == INVALID_SOCKET) return;

   closesocket(udp_socket->_socket);
   free(udp_socket);

   udp_socket_wsa_users--;
   if (udp_socket_wsa_users == 0) {
      WSACleanup();
      ZeroMemory(&udp_socket_wsa_data, sizeof(WSADATA));
   }
}

int32_t udp_socket_send(udp_socket_t *udp_socket, char *data, int size) {
   if (udp_socket_wsa_users == 0) return -1;
   if (udp_socket == NULL) return -1;

   int32_t bytes_sent = sendto(
      udp_socket->_socket, data, size, 0,
      (struct sockaddr *)&udp_socket->_address,
      udp_socket_address_size);

   if (bytes_sent == SOCKET_ERROR) return -1;
   return bytes_sent;
}

int32_t udp_socket_receive(udp_socket_t *udp_socket, char *data, const int size) {
   if (udp_socket_wsa_users == 0) return -1;
   if (udp_socket == NULL) return -1;

   const int32_t bytes_received = recvfrom(
      udp_socket->_socket, data, size, 0,
      (struct sockaddr *)&udp_socket->_address,
      &udp_socket_address_size);

   if (bytes_received == SOCKET_ERROR) return -1;
   return bytes_received;
}

int32_t udp_socket_get_last_error(udp_socket_t *udp_socket) {
   if (udp_socket_wsa_users == 0) return -1;
   if (udp_socket == NULL) return -1;
   return WSAGetLastError();
}
