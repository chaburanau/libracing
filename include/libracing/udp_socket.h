#pragma once

#include <stdint.h>

typedef struct UDPSocket udp_socket_t;

udp_socket_t *udp_socket_create(const char* address, int port);
void udp_socket_destroy(udp_socket_t *udp_socket);
int32_t udp_socket_send(udp_socket_t *udp_socket, char *data, int size);
int32_t udp_socket_receive(udp_socket_t *udp_socket, char *data, int size);
int32_t udp_socket_get_last_error(udp_socket_t *udp_socket);
