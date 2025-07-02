#pragma once

#include <stdint.h>
#include <winsock2.h>

typedef struct {
    SOCKET      _socket;
    SOCKADDR_IN _address;
} udp_socket_t;


udp_socket_t* udp_socket_create(const char* address, int port);
void udp_socket_close(udp_socket_t *udp_socket);

int32_t udp_socket_send(udp_socket_t *udp_socket, const char *data, int size);
int32_t udp_socket_receive(udp_socket_t *udp_socket, char *data, int size);