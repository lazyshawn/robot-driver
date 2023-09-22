#include "driver_core.h"
// bzero, memset
#include <string.h>

const double pi = 3.1415926535897932384626433832795028841971693993751058209;

// cart: [p, wxyz]
struct MotionStream {
  int size = 72;
  int msg_type = -1;
  int comm_type = 2;
  // 6 - moveJ, 7 - moveL
  int mov_type = 6;
  // 11 - joint, 12 - cart
  int coord_type = 11;
  float velocity = 100;
  int acc = 100;
  int smoot = 100;
  int trig_signal = 0;
  int reserved1 = 0;
  int reserved2 = 0;
  int reserved3 = 0;
  float pos[7];
};

// std::string robot_ip = "192.168.60.116";
std::string robot_ip = "192.168.20.10";
const int io_port = 54610;
// const int status_port = 54611;
const size_t status_port = 44818;
// const int status_port = 10031;
const size_t motion_port = 54612;


int create_socket(std::string ip, size_t port);
int read_socket();
int send_socket();

int main(int argc, char **argv) {
  // send_socket();
  read_socket();

  return 0;
}

int send_socket() {
  printf("Connecting to Robot: %s : %zd\n", robot_ip.c_str(), motion_port);

  int clientFd = create_socket(robot_ip, motion_port);
  if (clientFd < 1) {
    printf("# send_socket(): Cannot create socket.\n");
    return -1;
  }
  std::cout << "clientFd = " << clientFd << std::endl;

  // * 发送 socket 消息
  char motionsendbuffer[100];

  MotionStream mss, mss_recv;
  float pos[7] = {0,-30,0,0,-90,30,0};
  for (size_t i=0; i<7; ++i) {
    mss.pos[i] = pos[i] * pi / 180;
  }

  memcpy(motionsendbuffer, &mss, mss.size);
  for (int i = 0; i < mss.size; i += 4) {
    char temp0 = motionsendbuffer[i];
    char temp3 = motionsendbuffer[i + 3];
    motionsendbuffer[i + 3] = temp0;
    motionsendbuffer[i] = temp3;
    char temp1 = motionsendbuffer[i + 1];
    char temp2 = motionsendbuffer[i + 2];
    motionsendbuffer[i + 1] = temp2;
    motionsendbuffer[i + 2] = temp1;
  }

  // 发送 socket 信息
  if (write(clientFd, motionsendbuffer, mss.size) <= 0) {
    printf("Error: # send_socket(): cannot write to server.");
    return false;
  }

  // 接收反馈信息
  if (read(clientFd, motionsendbuffer, mss.size) <= 0) {
    return false;
  }
  for (int i = 0; i < mss.size; i += 4) {
    char temp0 = motionsendbuffer[i];
    char temp3 = motionsendbuffer[i + 3];
    motionsendbuffer[i + 3] = temp0;
    motionsendbuffer[i] = temp3;
    char temp1 = motionsendbuffer[i + 1];
    char temp2 = motionsendbuffer[i + 2];
    motionsendbuffer[i + 1] = temp2;
    motionsendbuffer[i + 2] = temp1;
  }
  memcpy(&mss_recv, motionsendbuffer, mss.size);
  printf("Info: # send_socket(): return success (%d), %s\n", mss_recv.msg_type, robot_ip.c_str());

  return 0;
}

int read_socket() {
  printf("Connecting to Robot: %s : %zd\n", robot_ip.c_str(), status_port);

  int clientFd = create_socket(robot_ip, status_port);
  if (clientFd < 1) {
    printf("# read_socket(): Cannot create socket.\n");
    return -1;
  }
  std::cout << "clientFd = " << clientFd << std::endl;

  // * 接受 socket 信息
  char statusrecvbuf[512] = {};
  memset(statusrecvbuf, 0, sizeof(statusrecvbuf));
  int recv_size = 28 + 6 * 8;
  if (recv(clientFd, statusrecvbuf, recv_size, 0) <= 0) {
    printf("status client recv fail: %s\n", robot_ip.c_str());
    return -1;
  }

  // if(fanuc_driver_info_.bytestream_switch)
  // 数据流每四位倒序
  for (int i = 0; i < recv_size; i += 4) {
    char temp0 = statusrecvbuf[i];
    char temp3 = statusrecvbuf[i + 3];
    statusrecvbuf[i + 3] = temp0;
    statusrecvbuf[i] = temp3;
    char temp1 = statusrecvbuf[i + 1];
    char temp2 = statusrecvbuf[i + 2];
    statusrecvbuf[i + 1] = temp2;
    statusrecvbuf[i + 2] = temp1;
  }

  std::cout << "\n==> Contents after bytestream_switch:" << std::endl;
  // 数据数量
  int size;
  memcpy(&size, statusrecvbuf, 4);
  // 关节位置
  float jpos[6];
  memcpy(jpos, statusrecvbuf+4, 24);
  // 笛卡尔空间位置
  float cartpos[7];
  memcpy(cartpos, statusrecvbuf+4+24, 24);
  // torq
  float torq[7];
  memcpy(torq, statusrecvbuf+28+24, 24);

  printf("size = %d", size);
  std::cout << "\njpose = " << std::endl;
  for (size_t i=0; i<6; ++i) {
    std::cout << jpos[i] << ", ";
  }
  std::cout << "\ncartpos = " << std::endl;
  for (size_t i=0; i<6; ++i) {
    std::cout << cartpos[i] << ", ";
  }
  std::cout << "\ntorq = " << std::endl;
  for (size_t i=0; i<6; ++i) {
    std::cout << torq[i] << ", ";
  }
  std::cout << std::endl;

  std::cout << "Client closed success." << std::endl;
  close(clientFd);
  return 0;
}

int create_socket(std::string ip, size_t port) {
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
    return -1;
  }

  return clientFd;
}

