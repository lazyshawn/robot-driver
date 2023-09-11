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
  mqttCli = std::make_shared<mqtt::async_client>(serverAddr, "");
  mqttCli->connect()->wait();
  std::cout << "Connected to MQTT server: " << serverAddr << std::endl;

  // HTTP 客户端初始化
  httpCli = std::make_shared<httplib::Client>("localhost", 9000);
}

Builder::~Builder() {
  // Disconnect
  std::cout << "Disconnecting..." << std::endl;
  mqttCli->disconnect()->wait();
  std::cout << "  ...OK" << std::endl;
}

void Builder::send_command(const std::vector<double>& joint) {
  json["robots"][0]["joint_state"] = joint;
  std::cout << "Publishing messages..." << std::endl;
  std::cout << json.dump(0) << std::endl;

  try {
    mqtt::topic top(*mqttCli, TOPIC.c_str(), QOS);
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
    mqtt::topic top(*mqttCli, TOPIC.c_str(), QOS);
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
  std::string postData = "{\"poseType\":1}";
  httplib::Result res = httpCli->Post("/query/robot_pos", postData.c_str(), "application/json");
  std::cout << res->body << std::endl;
  if (!(res->status == 200)) {
    printf("Warning: # Builder::read_status: Post failed.\n");
    return;
  }
}

