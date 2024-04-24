#include "fanuc/eip_driver.h"
#include <iostream>

int list_device();
int implicit_messaging();

using namespace eipScanner::cip;
using eipScanner::ConnectionManager;
using eipScanner::MessageRouter;
using eipScanner::SessionInfo;
using eipScanner::cip::connectionManager::ConnectionParameters;
using eipScanner::cip::connectionManager::NetworkConnectionParams;
using eipScanner::utils::Buffer;
using eipScanner::utils::Logger;
using eipScanner::utils::LogLevel;

const std::string ip = "192.168.1.12";
const int port = 0xAF12;

int main(int argc, char **argv) {
  list_device();
  implicit_messaging();
}

int list_device() {
  Logger::setLogLevel(LogLevel::DEBUG);

  eipScanner::DiscoveryManager discoveryManager(ip.c_str(), port, std::chrono::seconds(1));
  auto devices = discoveryManager.discover();

  for (auto& device : devices) {
    Logger(LogLevel::INFO) << "Discovered device: "
      << device.identityObject.getProductName()
      << " with address " << device.socketAddress.toString();
  }
  return EXIT_SUCCESS;
}

int implicit_messaging() {
  Logger::setLogLevel(LogLevel::DEBUG);
  // 创建会话
  auto si = std::make_shared<SessionInfo>(ip.c_str(), port);

  // Implicit messaging
  ConnectionManager connectionManager;

  ConnectionParameters parameters;
  // config AssmXXX, output AssmXXX, input AssmXXX
  parameters.connectionPath = { 0x20, 0x04, 0x24, 0x6A, 0x2C, 0x68,  0x2C, 0x69};
  parameters.o2tRealTimeFormat = true;
  parameters.originatorVendorId = 0x1234;
  parameters.originatorSerialNumber = 0x12345678;
  parameters.t2oNetworkConnectionParams |= NetworkConnectionParams::P2P;
  parameters.t2oNetworkConnectionParams |= NetworkConnectionParams::SCHEDULED_PRIORITY;
  parameters.t2oNetworkConnectionParams |= 32; // size of Assm100 = 32
  parameters.o2tNetworkConnectionParams |= NetworkConnectionParams::P2P;
  parameters.o2tNetworkConnectionParams |= NetworkConnectionParams::SCHEDULED_PRIORITY;
  parameters.o2tNetworkConnectionParams |= 32; // size of Assm150 = 32

  parameters.originatorSerialNumber = 0x12345678;
  parameters.o2tRPI = 10000;
  parameters.t2oRPI = 10000;
  parameters.transportTypeTrigger |= NetworkConnectionParams::CLASS1;
  parameters.connectionTimeoutMultiplier = 4;

  auto io = connectionManager.forwardOpen(si, parameters);
  std::cout << "forwardOpen success." << std::endl;
  if (auto ptr = io.lock()) {
    // ptr->setDataToSend(std::vector<uint8_t>(32));

    ptr->setReceiveDataListener(
        [](auto realTimeHeader, auto sequence, auto data) {
          std::ostringstream ss;
          ss << "secNum=" << sequence << " data=";
          for (auto &byte : data) {
            ss << "[" << std::hex << (int)byte << "]";
          }

          Logger(LogLevel::INFO) << "Received: " << ss.str();
        });

    ptr->setCloseListener([]() { Logger(LogLevel::INFO) << "Closed"; });
  }

  int count = 200;
  while (connectionManager.hasOpenConnections() && count-- > 0) {
    connectionManager.handleConnections(std::chrono::milliseconds(100));
  }

  connectionManager.forwardClose(si, io);

  return 0;
}
