#pragma once

#include <mqtt/async_client.h>
#include <nlohmann/json.hpp>
#include <httplib.h>

#include "utils/interpolate.h"

class Builder {
private:
  // Robot base
  bool jointCouple = true;

  // MQTT clinet
  std::shared_ptr<mqtt::async_client> mqttCli;
  std::string TOPIC, serverAddr;
  const int QOS = 2;
  nlohmann::json json;

  // HTTP client
  std::shared_ptr<httplib::Client> httpCli;

  // Servo settings
  std::chrono::duration<double, std::milli> circleTime;
  uint8_t servoStatus;

public:
  Builder();
  ~Builder();

  /**
  * @brief : 设定机械臂关节角状态，立即变换到该状态 (实际关节角)
  *          Builder 中直接设定的为几何关节角
  * @param : jointState (deg)
  * @return: 
  */
  void set_joint_state(const std::vector<double>& jointState);
  /**
  * @brief : 读取当前关节位置 (实际关节角)
  *          Builder 中直接读取的为实际关节角
  * @param : joint - 输出关节角 (deg)
  * @return: 
  */
  bool read_joint_state(std::vector<double>& joint);
  /**
  * @brief : moveJ，发送插值的关节角 (实际关节角)
  * @param : endJoint - 目标关节角 (deg)
  * @param : velocity - 最大关节速度 (deg/s)
  * @return: 
  */
  void moveJ(const std::vector<double>& endJoint, double velocity);
};
