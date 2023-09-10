#include "sbot/builder.h"

Builder::Builder() {
  // MQTT 客户端初始化
  serverAddr = "tcp://localhost:1883";
  TOPIC = "robot/robot/r1";
  const auto TIMEOUT = std::chrono::seconds(10);

  // 消息初始化
  json["robots"][0]["joint_state"] = std::vector<double>(6,0.0);
  json["robots"][0]["io_state"] = std::vector<double>(8,0.0);
  json["robots"][0]["servo_state"] = std::vector<double>(2,0.0);

  // 匿名注册客户端
  cli = std::make_shared<mqtt::async_client>(serverAddr, "");
  cli->connect()->wait();
  std::cout << "Connected to MQTT server: " << serverAddr << std::endl;
}

Builder::~Builder() {
  // Disconnect
  std::cout << "Disconnecting..." << std::endl;
  cli->disconnect()->wait();
  std::cout << "  ...OK" << std::endl;
}

void Builder::send_command(const std::vector<double>& joint) {
  json["robots"][0]["joint_state"] = joint;
  std::cout << "Publishing messages..." << std::endl;
  std::cout << json.dump(0) << std::endl;

  try {
    mqtt::topic top(*cli, TOPIC.c_str(), QOS);
    mqtt::token_ptr tok;
    tok = top.publish(json.dump());
    // Just wait for the last one to complete.
    tok->wait();
    std::cout << "OK" << std::endl;
  } catch (const mqtt::exception &exc) {
    std::cerr << exc << std::endl;
  }
}

void Builder::send_command_queue(const std::vector<std::vector<double>>& jointQueue) {
  try {
    mqtt::topic top(*cli, TOPIC.c_str(), QOS);
    mqtt::token_ptr tok;
    for (auto& joint : jointQueue) {
      json["robots"][0]["joint_state"] = joint;
      tok = top.publish(json.dump());
    }
    // Just wait for the last one to complete.
    tok->wait();
    std::cout << "OK" << std::endl;
  } catch (const mqtt::exception &exc) {
    std::cerr << exc << std::endl;
  }
}

void Builder::read_status(std::vector<double>& joint) {
}

