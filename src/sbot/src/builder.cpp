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

  circleTime = std::chrono::duration<double, std::milli>(100);
  printf("Circle time: %f ms.\n", circleTime.count());
}

Builder::~Builder() {
  // Disconnect
  std::cout << "Disconnecting..." << std::endl;
  mqttCli->disconnect()->wait();
  std::cout << "  ...OK" << std::endl;
}

void Builder::set_joint_state(const std::vector<double>& joint) {
  json["robots"][0]["joint_state"] = joint;
  std::cout << "Publishing messages..." << std::endl;
  std::cout << json.dump(0) << std::endl;

  try {
    mqtt::topic top(*mqttCli, TOPIC.c_str(), QOS);
    mqtt::token_ptr tok;
    tok = top.publish(json.dump());
    // Just wait for the last one to complete.
    tok->wait();
    printf("MQTT client disconnected.\n");
  } catch (const mqtt::exception &exc) {
    printf("%s", exc.what());
  }
}

bool Builder::read_joint_state(std::vector<double>& joint) {
  std::string postData = "{\"poseType\":1}";
  httplib::Result res = httpCli->Post("/query/robot_pos", postData.c_str(), "application/json");
  nlohmann::json resJson = res->body;
  // std::cout << res->body << std::endl;
  if (!(res->status == 200)) {
    printf("Warning: # Builder::read_status: Post failed.\n");
    return false;
  } else {
    joint = resJson["data"].get<std::vector<double>>();
  }
  return true;
}

void Builder::moveJ(const std::vector<double>& endJoint, double velocity) {
  std::vector<double> begJoint(6, 0);
  // if (read_joint_state(begJoint)) {
  //   printf("Error: # Builder::moveJ() : cannot read joint state;\n");
  //   return;
  // }

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
    for (size_t i=0; i<viaPoint.size(); ++i) {
      time_point<system_clock> cur = system_clock::now();
      if (cur > stamp + circleTime) {
        printf("Warning: phase skipped!\n");
        continue;
      }
      stamp = cur + circleTime;

      // Do servo things
      json["robots"][0]["joint_state"] = viaPoint[i];
      tok = top.publish(json.dump());

      // Finish all job, fall sleep
      std::this_thread::sleep_until(stamp);
    }

    // Just wait for the last one to complete.
    tok->wait();
    std::cout << "OK" << std::endl;
  } catch (const mqtt::exception &exc) {
    std::cerr << exc << std::endl;
  }
}

