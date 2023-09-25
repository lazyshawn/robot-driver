#include "utils/socket.h"

int create_socket_client(std::string ip, size_t port, long int delayMs) {
  int clientFd = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in targetAddr;
  // * 设置 sockaddr_in 结构体
  memset(&targetAddr, 0, sizeof(targetAddr));
  targetAddr.sin_family = AF_INET;
  targetAddr.sin_port = htons(port);
  targetAddr.sin_addr.s_addr = inet_addr(ip.c_str());

  // 设置通信超时 {s, ms}
  struct timeval send_read_to{15,0};
  setsockopt(clientFd, SOL_SOCKET, SO_SNDTIMEO, (const char *)&send_read_to, sizeof(send_read_to));
  setsockopt(clientFd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&send_read_to, sizeof(send_read_to));

  // * 建立 socket 连接
  if (::connect(clientFd, (struct sockaddr *)&targetAddr, sizeof(targetAddr)) + 1 == 0) {
    printf("Error: # create_socket(): Socket connect failed! ");
#ifdef WIN32
    closesocket(clientFd);
#else
    close(clientFd);
#endif
    clientFd = -1;
  }

  printf("Info: # create_socket_client(): connected to %s: %zd.\n", ip.c_str(), port);
  return clientFd;
}

int create_socket_server(std::string ip, size_t port) {
  // Create a socket to listening for incoming connection request
  int serverFd = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in targetAddr;
  // * 设置 sockaddr_in 结构体
  memset(&targetAddr, 0, sizeof(targetAddr));
  targetAddr.sin_family = AF_INET;
  targetAddr.sin_port = htons(port);
  targetAddr.sin_addr.s_addr = inet_addr(ip.c_str());

  if(bind(serverFd, (struct sockaddr *)&targetAddr, sizeof(targetAddr)) < 0) {
    printf("Error: # create_socket_server(): bind failed. ");
    return -1;
  }

  // 监听套接字, 队列长度为5
  if(::listen(serverFd, 5) < 0) {
    printf("Error: # create_socket_server(): listen failed. ");
  }

  printf("Info: # create_socket_server(): waiting for connection to %s: %zd ...\n", ip.c_str(), port);
  return serverFd;
}
