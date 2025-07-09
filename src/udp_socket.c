#include "../include/libracing/udp_socket.h"

#include <winerror.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

int udp_socket_address_size = sizeof(SOCKADDR_IN);

static WSADATA udp_socket_wsa_data;
static int udp_socket_wsa_users = 0;

udp_socket_init_status_t udp_socket_connect(udp_socket_t* udp_socket, const char* address, const int port) {
   udp_socket_init_status_t status = {0};

   udp_socket->_socket = INVALID_SOCKET;
   udp_socket->_address = (SOCKADDR_IN){0};

   if (udp_socket_wsa_users == 0) {
      const int wsa_startup_status = WSAStartup(MAKEWORD(2, 2), &udp_socket_wsa_data);
      if (wsa_startup_status != 0) {
         status.last_error = WSAGetLastError();
         udp_socket_close(udp_socket);
         return status;
      }
   }

   udp_socket_wsa_users++;

   udp_socket->_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (udp_socket->_socket == INVALID_SOCKET) {
      status.last_error = WSAGetLastError();
      udp_socket_close(udp_socket);
      return status;
   }

   ZeroMemory(&udp_socket->_address, udp_socket_address_size);
   udp_socket->_address.sin_family = AF_INET;
   udp_socket->_address.sin_port = htons(port);

   if (inet_pton(AF_INET, address, &udp_socket->_address.sin_addr) != 1) {
      status.last_error = WSAGetLastError();
      udp_socket_close(udp_socket);
      return status;
   }

   return status;
}

udp_socket_init_status_t udp_socket_close(udp_socket_t *udp_socket) {
   udp_socket_init_status_t status = {0};

   if (udp_socket == NULL) return status;

   if (udp_socket->_socket != INVALID_SOCKET) {
      closesocket(udp_socket->_socket);
      udp_socket->_socket = INVALID_SOCKET;
   }

   udp_socket_wsa_users--;

   if (udp_socket_wsa_users == 0) {
      WSACleanup();
      ZeroMemory(&udp_socket_wsa_data, sizeof(WSADATA));
   }

   status.last_error = WSAGetLastError();
   return status;
}

udp_socket_io_status_t udp_socket_send(udp_socket_t *udp_socket, const char *data, const int size) {
   udp_socket_io_status_t status = {0};

   if (udp_socket_wsa_users == 0) return status;
   if (udp_socket == NULL) return status;

   status.bytes_sent = sendto(
      udp_socket->_socket, data, size, 0,
      (struct sockaddr *)&udp_socket->_address,
      udp_socket_address_size);

   if (status.bytes_sent == SOCKET_ERROR) {
      status.last_error = WSAGetLastError();
      return status;
   }

   return status;
}

udp_socket_io_status_t udp_socket_receive(udp_socket_t *udp_socket, char *data, const int size) {
   udp_socket_io_status_t status = {0};

   if (udp_socket_wsa_users == 0) return status;
   if (udp_socket == NULL) return status;

   status.bytes_received = recvfrom(
      udp_socket->_socket, data, size, 0,
      (struct sockaddr *)&udp_socket->_address,
      &udp_socket_address_size);

   if (status.bytes_received == SOCKET_ERROR) {
      status.last_error = WSAGetLastError();
      return status;
   }

   return status;
}
