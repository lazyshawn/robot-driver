#pragma once

// socket 通信
#ifdef WIN32
#include <winsock2.h>
// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#else
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#endif
// bzero, memset
#include <string.h>

#include <string>

int create_socket_client(std::string ip, size_t port, long int delayMs = 0);
int create_socket_server(std::string ip, size_t port);
