#include <unitree/robot/go2/sport/sport_client.hpp>
#include <unistd.h>
#include <iostream>
#include <cmath>
#include <unitree/robot/channel/channel_subscriber.hpp>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " <networkInterface>" << std::endl;
    return -1;
  }

  unitree::robot::ChannelFactory::Instance()->Init(0, argv[1]);  // 改成你的网卡名，如 eth0 或 wlan0

  unitree::robot::go2::SportClient sport_client;
  sport_client.SetTimeout(10.0f);
  sport_client.Init();

  // 这里开始你的控制逻辑
  sport_client.StandUp();
  sport_client.SwitchJoystick(false);   // 关闭遥控器干扰（推荐）
  sport_client.BalanceStand();
  sleep(3);                      // 等待站稳

  // 测试移动
  for(int i = 0; i < 40; i++) {
    sport_client.Move(0.5f, 0.0f, 0.0f);
    usleep(100000);
  }
  sport_client.Move(0.0f, 0.0f, 0.0f);
  return 0;
}
