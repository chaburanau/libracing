#include "../include/libracing/udp_socket.h"

#include <stdio.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

int udp_socket_address_size = sizeof(SOCKADDR_IN);

static WSADATA udp_socket_wsa_data;
static int udp_socket_wsa_users = 0;

udp_socket_t* udp_socket_create(const char* address, const int port) {
   udp_socket_t* udp_socket = malloc(sizeof(udp_socket_t));
   udp_socket->_socket = INVALID_SOCKET;
   udp_socket->_address = { 0 };

   if (udp_socket_wsa_users == 0) {
      const int wsa_startup_status = WSAStartup(MAKEWORD(2, 2), &udp_socket_wsa_data);
      if (wsa_startup_status != 0) {
         fprintf(stderr, "UDP Socket: WSAStartup failed. Error: %d; %d\n", WSAGetLastError(), wsa_startup_status);
         udp_socket_close(udp_socket);
         return NULL;
      }
   }

   udp_socket_wsa_users++;

   udp_socket->_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (udp_socket->_socket == INVALID_SOCKET) {
      fprintf(stderr, "UDP Socket: socket failed. Error: %d\n", WSAGetLastError());
      udp_socket_close(udp_socket);
      return NULL;
   }

   ZeroMemory(&udp_socket->_address, udp_socket_address_size);
   udp_socket->_address.sin_family = AF_INET;
   udp_socket->_address.sin_port = htons(port);

   if (inet_pton(AF_INET, address, &udp_socket->_address.sin_addr) != 1) {
      fprintf(stderr, "UDP Socket: inet_pton failed. Error: %d\n", WSAGetLastError());
      udp_socket_close(udp_socket);
      return NULL;
   }

   return udp_socket;
}

void udp_socket_close(udp_socket_t *udp_socket) {
   if (udp_socket == NULL) return;

   if (udp_socket->_socket != INVALID_SOCKET) {
      closesocket(udp_socket->_socket);
      udp_socket->_socket = INVALID_SOCKET;
   }

   udp_socket_wsa_users--;

   if (udp_socket_wsa_users == 0) {
      WSACleanup();
      ZeroMemory(&udp_socket_wsa_data, sizeof(WSADATA));
   }

   free(udp_socket);
}

int32_t udp_socket_send(udp_socket_t *udp_socket, const char *data, const int size) {
   if (udp_socket == NULL) return -1;
   if (udp_socket_wsa_users == 0) return -1;

   const int sent = sendto(udp_socket->_socket, data, size, 0, (struct sockaddr *)&udp_socket->_address, udp_socket_address_size);
   if (sent == SOCKET_ERROR) {
      fprintf(stderr, "UDP Socket: sendto failed. Error: %d\n", WSAGetLastError());
      return -1;
   }

   return sent;
}

int32_t udp_socket_receive(udp_socket_t *udp_socket, char *data, const int size) {
   if (udp_socket == NULL) return -1;
   if (udp_socket_wsa_users == 0) return -1;

   const int received = recvfrom(udp_socket->_socket, data, size, 0, (struct sockaddr *)&udp_socket->_address, &udp_socket_address_size);
   if (received == SOCKET_ERROR) {
      fprintf(stderr, "UDP Socket: recvfrom failed. Error: %d\n", WSAGetLastError());
      return -1;
   }

   return received;
}