Go2 SDK 开发指南

搜索标题
软件服务接口
高层运动服务接口

高层运动服务接口


更新时间：2025-11-21 11:19:46
适用范围

本文档接口描述适用于 Go2 Edu 型号，要求 Go2 软件版本 < V1.1.6。

若您的软件版本 ≥ V1.1.6，请参考 《运控服务接口 V2.0》。
如有其他疑问，欢迎 联系官方技术支持。
感谢您的支持！

介绍
高层运动服务分为两个部分：高层控制接口、高层状态接口。

高层控制接口：通过调用  SDK   的 sport_client，来给  Go2  发送速度控制、轨迹跟踪等运动指令。
高层状态接口：通过订阅  SDK  中的 HigState 消息，来获取  Go2  的位置、速度、姿态等运动状态。
高层控制接口
高层控制接口的调用方式
#include <unitree/robot/go2/sport/sport_client.hpp>
#include <unistd.h>

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " networkInterface" << std::endl;
    exit(-1);
  }
  unitree::robot::ChannelFactory::Instance()->Init(0, argv[1]);
  //argv[1]由终端传入，为机器人连接的网卡名称
  
  //创建sport client对象
  unitree::robot::go2::SportClient sport_client;
  sport_client.SetTimeout(10.0f);//超时时间
  sport_client.Init();


  sport_client.Sit(); //特殊动作，机器狗坐下
  sleep(3);//延迟3s
  sport_client.RiseSit(); //恢复
  sleep(3);

  return 0;
}
高层运动控制接口介绍
高层控制接口可以实现  Go2  的姿态、速度和轨迹跟踪、空翻、跳跃、倒立等控制。下面列出运动控制接口的相关函数。

函数名	Damp
函数原型	int32_t Damp()
功能概述	进入阻尼状态。
参数	无
返回值	调用成功返回  0，否则返回相关错误码。
备注	所有电机关节停止运动并进入阻尼状态。该模式具有最高的优先级，用于突发情况下的急停。
函数名	BalanceStand
函数原型	int32_t BalanceStand()
功能概述	解除锁定。
参数	无
返回值	调用成功返回  0，否则返回相关错误码。
备注	解除关节电机锁定，从正常站立模式切换到平衡站立模式。这一模式下机身姿态和高度会始终保持平衡，不受地形的影响。可以通过调用 Euler()和 BodyHeight()接口实现机身的姿态控制(详见表格的对应部分)。
函数名	StopMove
函数原型	int32_t StopMove()
功能概述	停下当前动作，将绝大多数指令恢复成默认值。
参数	无
返回值	调用成功返回  0，否则返回相关错误码。
备注	停下当前的运动，并将 Go2 内部的运动参数恢复到默认值。
函数名	StandUp
函数原型	int32_t StandUp()
功能概述	关节锁定，站高。
参数	无
返回值	调用成功返回  0，否则返回相关错误码。
备注	机器狗正常站高，电机关节保持锁定。相比于平衡站立模式，该模式下机器狗的姿态不会始终保持平衡。默认的站立高度为 0.33m
函数名	StandDown
函数原型	int32_t StandDown()
功能概述	关节锁定，站低。
参数	无
返回值	调用成功返回  0，否则返回相关错误码。
备注	机器狗趴下，电机关节保持锁定。
函数名	RecoveryStand
函数原型	int32_t RecoveryStand()
功能概述	恢复站立。
参数	无
返回值	调用成功返回  0，否则返回相关错误码。
备注	从翻倒或趴下状态恢复至平衡站立状态。不论是否翻倒，都会恢复至站立。
函数名	Euler
函数原型	int32_t Euler(float roll, float pitch, float yaw)
功能概述	站立和行走时的姿态。
参数	roll:  取值范围  [-0.75~0.75] (rad)； pitch:  取值范围  [-0.75~0.75] (rad)； yaw:  取值范围  [-0.6~0.6] (rad)。
返回值	调用成功返回  0，否则返回相关错误码。
备注	设置 Go2 平衡站立或移动时的机体姿态角。欧拉角采用绕机体相对轴和 z-y-x 旋转顺序的表示方式。
函数名	Move
函数原型	int32_t Move(float vx, float vy, float vyaw)
功能概述	移动。
参数	vx:  取值范围[-2.5~3.8] (m/s)； vy:  取值范围[-1.0~1.0] (m/s)； vyaw:  取值范围[-4~4] (rad/s)。
返回值	调用成功返回  0，否则返回相关错误码。
备注	控制移动速度。设定的速度为机体坐标系表示下的速度。
函数名	Sit
函数原型	int32_t Sit();
功能概述	坐下。
参数	无
返回值	调用成功返回  0，否则返回相关错误码。
备注	特殊动作，机器狗坐下。需要注意特殊动作需要在上一个动作执行完毕后再执行，否则会导致动作异常。
函数名	RiseSit
函数原型	int32_t RiseSit()
功能概述	站起（相对于坐下）。
参数	无
返回值	调用成功返回  0，否则返回相关错误码。
备注	从坐下状态恢复到平衡站立
函数名	SwitchGait
函数原型	int32_t SwitchGait(int d)
功能概述	切换步态。
参数	d： 步态枚举值，取值  0~4，0  为  idle， 1  为  trot，2  为  trot running，3  正向爬楼模式，4：逆向爬楼模式。
返回值	调用成功返回  0，否则返回相关错误码。
备注	正向爬楼模式为机器人头朝楼梯，逆向爬楼模式为机器人背朝楼梯
函数名	BodyHeight
函数原型	int32_t BodyHeight(float height)
功能概述	设置站立和行走时的机身离地高度相对值。
参数	height： 与默认机身高度相对值，取值[-0.18~0.03] (m)
返回值	调用成功返回  0，否则返回相关错误码。
备注	调节平衡站立或行走时的机身相对与默认状态的高度，Go2  默认机身高度  0.33。例如： BodyHeight(-0.1)表示调节机身高度至 0.33-0.1=0.23 (m)。
函数名	FootRaiseHeight
函数原型	int32_t FootRaiseHeight(float height)
功能概述	设置移动时的抬足相对高度。
参数	height： 与默认抬足高度相对值，取值[-0.06~0.03] (m)
返回值	调用成功返回  0，否则返回相关错误码。
备注	设置 Go2 运动时相对于默认状态的抬腿高度，Go2  默认抬足高度  0.09。例如：FootRaiseHeight(0.01)表示设置抬腿高度为 0.09+0.01=0.1 (m)
函数名	SpeedLevel
函数原型	int32_t SpeedLevel(int level)
功能概述	设置速度档位。
参数	level：速度档位枚举值，取值  -1  为慢速，0  为正常，1  为快速。
返回值	调用成功返回  0，否则返回相关错误码。
备注	
函数名	Hello
函数原型	int32_t Hello()
功能概述	打招呼。
参数	无
返回值	调用成功返回  0，否则返回相关错误码。
备注	
函数名	Stretch
函数原型	int32_t Stretch()
功能概述	伸懒腰。
参数	无
返回值	调用成功返回  0，否则返回相关错误码。
备注	
函数名	TrajectoryFollow
函数原型	int32_t TrajectoryFollow(std::vector path);
功能概述	轨迹跟踪。
参数	path 是机器狗在未来一段时间内的目标运动轨迹，它由 30 个轨迹点 PathPoint 组成。
返回值	调用成功返回  0，否则返回相关错误码。
备注	path 中轨迹点()PathPoint)的结构体定义如下： 每个轨迹点包含了机器狗在未来某个时刻的位置、速度，参考坐标系为以机器人的绝对坐标系。通过轨迹跟踪的方式，能够实现更加复杂和灵活的运动。具体的使用方式可以参考例子。
函数名	SwitchJoystick
函数原型	int32_t SwitchJoystick(bool flag)
功能概述	原生遥控器响应开关。
参数	flag：设置 true  为响应原生遥控器，false  为不响应遥控器。
返回值	调用成功返回  0，否则返回相关错误码。
备注	关闭遥控器响应后，遥控器的指令不会干涉当前程序的运行
函数名	ContinuousGait
函数原型	int32_t ContinuousGait(bool flag)
功能概述	持续移动。
参数	flag：设置 true  为开启，false  为关闭。
返回值	调用成功返回  0，否则返回相关错误码。
备注	启动连续步态后，机器狗会不断保持踏步状态，即使当前速度为 0。
函数名	Wallow
函数原型	int32_t Wallow()
功能概述	打滚。
参数	无
返回值	调用成功返回  0，否则返回相关错误码。
备注	
函数名	Pose
函数原型	int32_t Pose(bool flag)
功能概述	摆姿势。
参数	flag：设置 true  为摆姿式，false  为恢复。
返回值	调用成功返回  0，否则返回相关错误码。
备注	
函数名	Scrape
函数原型	int32_t Scrape()
功能概述	拜年作揖。
参数	无
返回值	调用成功返回  0，否则返回相关错误码。
备注	
函数名	FrontFlip
函数原型	int32_t FrontFlip()
功能概述	前空翻。
参数	无
返回值	调用成功返回  0，否则返回相关错误码。
备注	
函数名	FrontJump
函数原型	int32_t FrontJump()
功能概述	前跳。
参数	无
返回值	调用成功返回  0，否则返回相关错误码。
备注	
函数名	FrontPounce
函数原型	int32_t FrontPounce()
功能概述	向前扑人。
参数	无
返回值	调用成功返回  0，否则返回相关错误码。
备注	
函数名	Dance1
函数原型	int32_t Dance1()
功能概述	舞蹈段落  1。
参数	无
返回值	调用成功返回  0，否则返回相关错误码。
备注	
函数名	Dance2
函数原型	int32_t Dance2()
功能概述	舞蹈段落  2。
参数	无
返回值	调用成功返回  0，否则返回相关错误码。
备注	
函数名	GetState
函数原型	int32_t GetState(const std::vectorstd::string& _vector, std::map<std::string, std::string>& _map);
功能概述	获取机器人当前状态，包括：
state: 运动状态
bodyHeight：机体高度,
footRaiseHeight：抬腿高度,
speedLevel：速度档位,
gait 步态,
joystick： 遥控器状态,
dance：是否处于动作中,
continuousGait：是否处于连续步态
economicGait：是否处于省电步态
参数	_vector：机器人状态名称的vector引用；_map： 状态名和对应的值的map引用。
返回值	调用成功返回 0，否则返回相关错误码。
备注	参考例子：当机器人处于阻尼状态时，上述例子会输出 {"data":"damping"} 的结果。
函数名	MoveToPos
函数原型	int32_t MoveToPos(float x, float y, float yaw)
功能概述	移动到里程计坐标系中指定位置。
参数	x: 里程计坐标系中的x，单位m；y: 里程计坐标系中的y，单位m；yaw: 里程计坐标系中的偏航角，单位rad，建议取值范围-3.14～3.14。
返回值	调用成功返回  0，否则返回相关错误码。
备注	位姿(x，y，ywa)是相对于里程计坐标系。
函数名	HandStand
函数原型	int32_t HandStand(bool flag)
功能概述	倒立行走。
参数	flag：设置 true  为开启，false  为关闭。
返回值	调用成功返回  0，否则返回相关错误码。
备注	进入倒立模式。该模式需要在可移动状态下，才能进入。发送StandOut前，可先发送BalanceStand()，确保可切入倒立模式
函数名	LeftFlip
函数原型	int32_t LeftFlip()
功能概述	左空翻。
参数	无。
返回值	调用成功返回  0，否则返回相关错误码。
备注	进行单次左空翻，空翻结束后会自动进入灵动模式。该模式需要在非运动状态下才能进入。
函数名	BackFlip
函数原型	int32_t BackFlip()
功能概述	后空翻。
参数	无。
返回值	调用成功返回  0，否则返回相关错误码。
备注	进行单次后空翻，空翻结束后会自动进入灵动模式。该模式需要在非运动状态下才能进入。
函数名	FreeWalk
函数原型	int32_t FreeWalk()
功能概述	灵动模式。
参数	无。
返回值	调用成功返回  0，否则返回相关错误码。
备注	进入灵动模式。
函数名	FreeBound
函数原型	int32_t FreeBound(bool flag)
功能概述	并腿跑模式。
参数	flag：设置true，进入并腿跑；设置false，退出并腿跑，进入灵动。
返回值	调用成功返回  0，否则返回相关错误码。
备注	进入并腿跑模式。
函数名	FreeJump
函数原型	int32_t FreeJump(bool flag)
功能概述	跳跃模式。
参数	flag：设置true，进入跳跃模式；设置false，退出跳跃模式，进入灵动。
返回值	调用成功返回  0，否则返回相关错误码。
备注	进人跳跃模式。
函数名	FreeAvoid
函数原型	int32_t FreeAvoid(bool flag)
功能概述	闪避模式。
参数	flag：设置true，进入闪避模式；设置false，退出闪避模式，进入灵动。
返回值	调用成功返回  0，否则返回相关错误码。
备注	进入闪避模式。该模式下，机器人移动时，可进行避障；静止状态下，可对前方物体进行闪避。
函数名	WalkStair
函数原型	int32_t WalkStair(bool flag)
功能概述	爬楼梯模式。
参数	flag：设置true，进入爬楼梯模式；设置false，退出爬楼梯模式，进入灵动。
返回值	调用成功返回  0，否则返回相关错误码。
备注	进入爬楼梯模式。
函数名	WalkUpright
函数原型	int32_t WalkUpright(bool flag)
功能概述	后腿直立模式。
参数	flag：设置true，进入后腿直立模式；设置false，退出后腿直立模式，进入灵动。
返回值	调用成功返回  0，否则返回相关错误码。
备注	进入后腿直立模式。
函数名	CrossStep
函数原型	int32_t CrossStep(bool flag)
功能概述	交叉步模式。
参数	flag：设置true，进入交叉步模式；设置false，退出交叉步模式，进入灵动。
返回值	调用成功返回  0，否则返回相关错误码。
备注	进入交叉步模式。
其中接口错误码如下：

错误号	错误描述
4101	轨迹点数错误，由客户端返回
4201	动作超时错误，在期望的时间内没有完成指定动作
4205	状态机未初始化结束
4206	执行挥手或拜年类动作前，机器人姿态不佳，不予执行
3104	DDS 超时
实例
下面给出一个控制机器狗实现站立时姿态控制的例子

#include <cmath>
#include <signal.h>
#include <unistd.h>

#include <unitree/robot/go2/sport/sport_client.hpp>

bool stopped = false;

void sigint_handler(int sig)
{
  if (sig == SIGINT)
  {
    stopped = true;
  }
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " networkInterface" << std::endl;
    exit(-1);
  }
  unitree::robot::ChannelFactory::Instance()->Init(0, argv[1]);
  //argv[1]由终端传入，为机器人连接的网卡名称

  //创建一个sport_client对象
  unitree::robot::go2::SportClient sport_client;
  sport_client.SetTimeout(10.0f); //超时时间
  sport_client.Init(); //初始化sport client

  double t = 0; //程序运行时间
  double dt = 0.01; //控制步长

  //创建一个signal对象用于捕获程序退出的信号
  signal(SIGINT, sigint_handler);

  while (1)
  {
    t += dt;
    //姿态跟踪一个三角函数轨迹
    sport_client.Euler(0.2 * sin(2 * t), 0.2 * cos(2 * t) - 0.2, 0.);
    sport_client.BalanceStand();
    usleep(int(dt * 1000000));
    if(stopped)
    {
      break;
    }

  }
  //程序退出时复位姿态
  sport_client.Euler(0, 0, 0);
  return 0;
}
高层状态接口
高层状态接口调用方式
通过订阅"rt/sportmodestate"话题可以实现对机器人的位置、速度、姿态等运动状态的获取。

#include <unitree/idl/go2/SportModeState_.hpp>
#include <unitree/robot/channel/channel_subscriber.hpp>

//高层状态topic，其中rt表示实时，lf表示低频
#define TOPIC_HIGHSTATE "rt/sportmodestate"

using namespace unitree::robot;

//获取运动状态的回调函数
void HighStateHandler(const void* message)
{
  unitree_go::msg::dds_::SportModeState_ state = *(unitree_go::msg::dds_::SportModeState_*)message;

  //打印输出机器狗位置
  std::cout<<"position: "
           <<state.position()[0]<<", "
           <<state.position()[1]<<", "
           <<state.position()[2]<<std::endl;
  //打印输出机器狗姿态四元数 (w,x,y,z)
  std::cout<<": "
           <<state.imu_state().quaternion()[0]<<", "
           <<state.imu_state().quaternion()[1]<<", "
           <<state.imu_state().quaternion()[2]<<", "
           <<state.imu_state().quaternion()[3]<<std::endl;
}


int main()
{
  //初始化sdk接口
  std::string networkInterface = "enp2s0";//机器人连接的网卡名称
  unitree::robot::ChannelFactory::Instance()->Init(0, networkInterface);

  //创建一个Subscriber
  ChannelSubscriber<unitree_go::msg::dds_::SportModeState_> suber(TOPIC_HIGHSTATE);

  //初始化Channel
  suber.InitChannel(HighStateHandler);

  while(1)
  {
    usleep(20000);
  }

  return 0;
}
高层状态类型介绍
获取高层部分状态的方法如下

TimeSpec stamp(); 
uint32_t errorCode(); //错误代码
IMU imuState(); //IMU状态
uint8_t mode(); //运动模式
/*
运动模式
0. idle, default stand
1. balanceStand
2. pose
3. locomotion
4. reserve
5. lieDown
6. jointLock
7. damping
8. recoveryStand
9. reserve
10. sit
11. frontFlip
12. frontJump
13. frontPounc
*/

float progress(); //是否动作执行状态：0. dance false; 1. dance true
uint8_t gaitType(); //步态类型
/*
步态类型
0.idle  
1.trot  
2.run  
3.climb stair  
4.forwardDownStair   
9.adjust
*/

float footRaiseHeight(); //抬腿高度
std::array<float, 3> position(); //三维位置
float bodyHeight(); //机体高度
std::array<float, 3> velocity(); //三维速度
float yawSpeed(); //偏航速度
std::array<float, 4> rangeObstacle(); //障碍物距离
std::array<int16_t, 4> footForce(); //四个足端力
std::array<float, 12> footPositionBody(); //足端相对于机体的位置
std::array<float, 12> footSpeedBody(); //足端相对与机体的速度
std::array<PathPoint, 10>  pathPoints(); //当前跟踪的路径点
其中获取  IMU  数据的方法为：

std::array<float, 4> quaternion();     // 四元数 (w,x,y,z)
std::array<float, 3> gyroscope();      // 角速度（unit: rad/s)
std::array<float, 3> accelerometer();  // 加速度 m/(s2)
std::array<float, 3> rpy();            // 欧拉角（unit: rad)
int8_t temperature(); //温度
PathPoint  的结构为：

typedef struct
{
  float tFromStart; //路径点所处时刻
  float x; //x位置
  float y; //y位置
  float yaw; //偏航角
  float vx; //x速度
  float vy; //y速度
  float vyaw; //偏航速度
} PathPoint;
