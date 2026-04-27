  #include <iostream>
#include <thread>
#include <chrono>
#include <unitree/robot/channel/channel_subscriber.hpp>
#include <unitree/idl/go2/SportModeState_.hpp>

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " <network_interface>" << std::endl;
    return -1;
  }

  unitree::robot::ChannelFactory::Instance()->Init(0, argv[1]);

  unitree::robot::ChannelSubscriber<unitree_go::msg::dds_::SportModeState_> stateSub("rt/sportmodestate");
  unitree_go::msg::dds_::SportModeState_ state;

  stateSub.InitChannel([&](const void* msg) {
      state = *(unitree_go::msg::dds_::SportModeState_*)(msg);
  }, 1);

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  std::cout << "Current mode: " << (int)state.mode() << std::endl;
  std::cout << "Gait type: " << (int)state.gait_type() << std::endl;

  return 0;
}