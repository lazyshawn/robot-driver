#pragma once

#include <mqtt/async_client.h>
#include <nlohmann/json.hpp>
#include <httplib.h>

class Builder {
private:
  // MQTT clinet
  std::shared_ptr<mqtt::async_client> mqttCli;
  std::string TOPIC, serverAddr;
  const int QOS = 1;
  nlohmann::json json;
  // HTTP client
  std::shared_ptr<httplib::Client> httpCli;

public:
  Builder();
  ~Builder();

  void send_command(const std::vector<double>& joint);
  void send_command_queue(const std::vector<std::vector<double>>& jointQueue);
  void read_status(std::vector<double>& joint);
};
