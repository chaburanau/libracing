#pragma once

#include <stdint.h>
#include <winsock2.h>

typedef struct {
    SOCKET _socket;
    SOCKADDR_IN _address;
} udp_socket_t;

typedef struct {
    int32_t last_error; // WSAGetLastError() value
} udp_socket_init_status_t;

typedef struct {
    uint32_t bytes_sent;
    uint32_t bytes_received;
    int32_t last_error; // WSAGetLastError() value
} udp_socket_io_status_t;

udp_socket_init_status_t udp_socket_connect(udp_socket_t *udp_socket, const char* address, int port);
udp_socket_init_status_t udp_socket_close(udp_socket_t *udp_socket);
udp_socket_io_status_t udp_socket_send(udp_socket_t *udp_socket, const char *data, int size);
udp_socket_io_status_t udp_socket_receive(udp_socket_t *udp_socket, char *data, int size);
