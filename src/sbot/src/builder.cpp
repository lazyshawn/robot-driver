#include "sbot/builder.h"

Builder::Builder() {
  // MQTT 客户端初始化
  serverAddr = "tcp://localhost:1883";
  TOPIC = "robot/robot/r1";
  const auto TIMEOUT = std::chrono::seconds(10);

  // 消息初始化
  json["robots"][0]["joint_state"] = std::vector<double>(6, 0.0);
  json["robots"][0]["io_state"] = std::vector<double>(8, 0.0);
  json["robots"][0]["servo_state"] = std::vector<double>(2, 0.0);

  // 匿名注册客户端
  mqttCli = std::make_shared<mqtt::async_client>(serverAddr, "");
  mqttCli->connect()->wait();
  std::cout << "Connected to MQTT server: " << serverAddr << std::endl;

  // HTTP 客户端初始化
  httpCli = std::make_shared<httplib::Client>("127.0.0.1", 9000);

  circleTime = std::chrono::duration<double, std::milli>(50);
  printf("Circle time: %f ms.\n", circleTime.count());
}

Builder::~Builder() {
  // Disconnect
  printf("Disconnecting from MQTT ...\n");
  mqttCli->disconnect()->wait();
  printf("MQTT client disconnected.\n");
}

void Builder::set_joint_state(const std::vector<double> &joint) {
  std::vector<double> targetJoint = joint;
  // 转为几何关节角
  if (jointCouple) {
    targetJoint[2] += targetJoint[1];
  }
  json["robots"][0]["joint_state"] = targetJoint;

  try {
    mqtt::topic top(*mqttCli, TOPIC.c_str(), QOS);
    mqtt::token_ptr tok;
    tok = top.publish(json.dump());
    // Just wait for the last one to complete.
    tok->wait();
  } catch (const mqtt::exception &exc) {
    printf("%s", exc.what());
  }
}

bool Builder::read_joint_state(std::vector<double> &joint) {
  std::string postData = "{\"poseType\":1}";
  httplib::Result res = httpCli->Post("/query/robot_pos", postData.c_str(), "application/json");
  nlohmann::json resJson = nlohmann::json::parse(res->body);

  if (!(res->status == 200)) {
    printf("Warning: # Builder::read_status: Post failed.\n");
    return false;
  } else {
    std::string dataStr = resJson["data"]["data"], tmp;
    std::istringstream in(dataStr);
    // json.get<>() 有问题，手动转换
    joint.clear();
    joint.reserve(7);
    while (getline(in, tmp, ',')) {
      joint.push_back(std::stod(tmp));
    }
  }
  return true;
}

void Builder::moveJ(const std::vector<double> &endJoint, double velocity) {
  // 读取当前关节角，需要在 builder 中选中机器人
  std::vector<double> begJoint(6, 0);
  if (!read_joint_state(begJoint)) {
    printf("Error: # Builder::moveJ() : cannot read joint state;\n");
    return;
  }

  // 伺服周期内的最大关节增量 rad/s -> rad/circleTime
  double maxStep = velocity / 1000 * circleTime.count();
  std::vector<std::vector<double>> viaPoint = get_linear_interpolate(begJoint, endJoint, maxStep);

  try {
    mqtt::topic top(*mqttCli, TOPIC.c_str(), QOS);
    mqtt::token_ptr tok;

    using namespace std::chrono;
    // 伺服线程开始，获取当前时刻
    time_point<system_clock, duration<double, std::milli>> stamp = system_clock::now();
    // Set next wake up time point
    stamp += circleTime;
    for (size_t i = 0; i < viaPoint.size(); ++i) {
      time_point<system_clock> cur = system_clock::now();
      if (cur > stamp + circleTime) {
        printf("Warning: phase skipped!\n");
        continue;
      }
      stamp = cur + circleTime;

      // 转为几何关节角
      if (jointCouple) {
        viaPoint[i][2] += viaPoint[i][1];
      }
      // Do servo things
      json["robots"][0]["joint_state"] = viaPoint[i];
      tok = top.publish(json.dump());

      // Finish all job, fall sleep
      std::this_thread::sleep_until(stamp);
    }

    // Just wait for the last one to complete.
    tok->wait();
  } catch (const mqtt::exception &exc) {
    printf("%s", exc.what());
  }
}
