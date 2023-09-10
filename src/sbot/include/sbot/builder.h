#pragma once

#include <mqtt/async_client.h>
#include <nlohmann/json.hpp>

class Builder {
private:
  std::shared_ptr<mqtt::async_client> cli;
  std::string TOPIC, serverAddr;
  const int QOS = 1;
  nlohmann::json json;

public:
  Builder();
  ~Builder();

  void send_command(const std::vector<double>& joint);
  void send_command_queue(const std::vector<std::vector<double>>& jointQueue);
  void read_status(std::vector<double>& joint);
};
