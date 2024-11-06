

#include "esp_camera.h"
#include <WiFi.h>
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "fb_gfx.h"
#include "soc/soc.h"          // disable brownout problems
#include "soc/rtc_cntl_reg.h" // disable brownout problems
#include "esp_http_server.h"

#include "CameraServer.h"
#include "FourWheelDrive.h"

/*
  ------------------------------ Diagram --------------------------------

                  DIR_UP                                     DIR_UP
                    ^                                          ^
                   /|\                                        /|\
                    |                                          |
                 ___|____                                   ___|____
                |        |                                 |        |
                |  Left  |                                 |  Right |
   DIR_LEFT <-- |Joystick| --> DIR_RIGHT      DIR_LEFT <-- |Joystick| --> DIR_RIGHT
                |________|                                 |________|
                    |                                          |
                   \|/                                        \|/
                    V                                          V
                 DIR_DOWN                                   DIR_DOWN

*/

// joystick value from -100 to 100
#define JOYSTICK_DIR_THRESHOLD (70)
#define JOYSTICK_DIR_P (1)  // positive
#define JOYSTICK_DIR_N (-1) // negative

/*
Joysitck 1: control ARM baseServo & LeftServo
Joystick 2: contorl ARM rightServo & ClawServo
*/
int joystick_value[JOYSTICK_INDEX_MAX];

struct obj_info obj_detect_info;

// #define WORK_MODE_AP
#define USE_FWD
#define USE_OBJ_DETECT

#ifdef WORK_MODE_AP
// Replace with your network credentials
const char *ssid = "somebot";
const char *password = "12345678";
#else
// Replace with your network credentials
const char *ssid = "somebot";
const char *password = "12345678.";
// 手机保持4G流量打开
#endif


#ifdef USE_FWD
FourWheelDrive FWD;
#endif

int servo_cur_angle[FWD_SERVO_MAX];  // 舵机当前的旋转角度
int servo_min_angle[FWD_SERVO_MAX];  // 允许舵机旋转到达的最小值
int servo_max_angle[FWD_SERVO_MAX];  // 允许舵机旋转到达的最大值
int servo_init_angle[FWD_SERVO_MAX]; // 刚上电时，舵机的初始角度

#define CLAW_OPEN_ANGLE (55)  // 爪子张开时的舵机角度
#define CLAW_CLOSE_ANGLE (10) // 爪子闭合时的舵机角度

#define ARM_SERVOS (FWD_ARM_SERVO_CLAW + 1)

int picking_actions[COLOR_MAX][8][ARM_SERVOS+1] = {
    { // red
        {90, 86, 58, CLAW_OPEN_ANGLE, 100}, 

        {90, 73, 113, CLAW_OPEN_ANGLE, 100},
        {90, 73, 113, CLAW_CLOSE_ANGLE, 100}, 

        {90, 140, 57, CLAW_CLOSE_ANGLE, 100}, 

        {134, 140, 72, CLAW_CLOSE_ANGLE, 100}, 
        {134, 140, 72, CLAW_OPEN_ANGLE, 100},  

        {90, 140, 57, CLAW_OPEN_ANGLE, 100}, 
        
        {90, 86, 58, CLAW_OPEN_ANGLE, 100},  
    },

    { // green
        {90, 86, 58, CLAW_OPEN_ANGLE, 100}, 

        {90, 73, 113, CLAW_OPEN_ANGLE, 100},
        {90, 73, 113, CLAW_CLOSE_ANGLE, 100}, 

        {90, 140, 57, CLAW_CLOSE_ANGLE, 100}, 

        {34, 140, 72, CLAW_CLOSE_ANGLE, 100}, 
        {34, 140, 72, CLAW_OPEN_ANGLE, 100},  

        {90, 140, 57, CLAW_OPEN_ANGLE, 100}, 
        
        {90, 86, 58, CLAW_OPEN_ANGLE, 100},  
    },

    { // blue
        {90, 86, 58, CLAW_OPEN_ANGLE, 100}, 

        {90, 73, 113, CLAW_OPEN_ANGLE, 100},
        {90, 73, 113, CLAW_CLOSE_ANGLE, 100}, 

        {90, 140, 57, CLAW_CLOSE_ANGLE, 100}, 

        {0, 140, 41, CLAW_CLOSE_ANGLE, 100}, 
        {0, 140, 41, CLAW_OPEN_ANGLE, 100},  

        {90, 140, 57, CLAW_OPEN_ANGLE, 100}, 
        
        {90, 86, 58, CLAW_OPEN_ANGLE, 100},  
    },

    { // grey
        {90, 86, 58, CLAW_OPEN_ANGLE, 100}, 

        {90, 73, 113, CLAW_OPEN_ANGLE, 100},
        {90, 73, 113, CLAW_CLOSE_ANGLE, 100}, 

        {90, 140, 57, CLAW_CLOSE_ANGLE, 100}, 

        {180, 140, 41, CLAW_CLOSE_ANGLE, 100}, 
        {180, 140, 41, CLAW_OPEN_ANGLE, 100},  

        {90, 140, 57, CLAW_OPEN_ANGLE, 100}, 
        
        {90, 86, 58, CLAW_OPEN_ANGLE, 100},  
    },
};

#define MAXSPEED    (10)    // 舵机最大旋转速度
int demo_speed = 1;         // 1倍速、2倍速......


void setup()
{
  esp_err_t err;

  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout detector

  Serial.begin(115200);
  Serial.setDebugOutput(false);

  Serial.printf("CONFIG_HTTPD_MAX_REQ_HDR_LEN=%d, CONFIG_HTTPD_MAX_URI_LEN=%d\n", CONFIG_HTTPD_MAX_REQ_HDR_LEN, CONFIG_HTTPD_MAX_URI_LEN);

  init_wifi();

  err = initCamServer();
  if (err != ESP_OK)
  {
    return;
  }

#ifdef USE_FWD
  init_FWD();
#endif
}

void loop()
{
#ifdef USE_FWD
#ifdef USE_OBJ_DETECT
  play_sorting_actions();
  
  // shall we open the FLASH LIGHT?
  if (getLedSwitchState())
  {
    FWD.setFlashLight(100);
  }
  else
  {
    FWD.setFlashLight(0);
  }

  delay(16);
#else
  do_control();
  delay(16);
#endif
#else
  delay(10);
#endif
}

void play_sorting_actions(void)
{
  obj_detect_info.color = COLOR_NONE;

  // 进入目标检测姿态
  move_from_to(servo_init_angle, picking_actions[COLOR_MIN][0]);

  // 进行目标检测--获取浏览器检测结果
  getObjInfo(&obj_detect_info);
  if (COLOR_NONE == obj_detect_info.color)
  {
    return;
  }    

  setObjSortingStatus(2); // 2 for sorting is executing;
  int color = obj_detect_info.color;
  // 计算总共有多少个动作？
  int counts = sizeof(picking_actions[color]) / sizeof(picking_actions[color][0]);
  Serial.printf("picking_actions[%d] counts=%d\n", color, counts);  

  for (int i = 0; i < counts - 1; i++)
  {    
    // 机械臂从当前动作，运动、转移至下一个动作
    move_from_to(picking_actions[color][i], picking_actions[color][i + 1]);
  }
  move_from_to(picking_actions[color][counts-1], picking_actions[color][0]);

  clearObjInfo();
  setObjSortingStatus(1); // 1 for idle(need detecting obj color then reply to ESP32);
}

/*
    move_from_to:
    目的是，让机械臂的2个动作之间的变化、运动、转移，更加平滑，
    不要让舵机瞬间完成角度切换，一方面增加舵机使用寿命，一方面机械臂工作更加自然。
*/
void move_from_to(int *action_from, int *action_to)
{
  int max_angle = 0;
  int max_steps = 0;
  float step_angle[ARM_SERVOS];


  /*
    1. 找到角度变化最大的那个舵机，算出角度变化绝对值max_angle；
    2. 角度变化值max_angle ÷ 舵机旋转速度demo_speed，就是舵机要运行的步数max_steps；
    3. 根据总步数max_steps，计算出每个舵机单步转动的角度step_angle；
  */
  max_angle = max(max(abs(action_to[0] - action_from[0]), abs(action_to[1] - action_from[1])), abs(action_to[2] - action_from[2]));
  max_steps = max_angle / demo_speed;
  max_steps = max_steps < 1 ? 1 : max_steps;
  for (int i = 0; i < FWD_ARM_SERVO_CLAW; i++)
  {
    step_angle[i] = float(action_to[i] - action_from[i]) / float(max_steps);
  }

  for (int j = 1; j <= max_steps; j++) // 步数j累加
  {
    for (int i = 0; i < FWD_ARM_SERVO_CLAW; i++)
    {
      // 随着j慢慢增大，new_angle 会慢慢趋近于 action_to[i]，也就实现了运动平滑的效果
      int new_angle = action_from[i] + j * step_angle[i];

      if ((new_angle < servo_min_angle[i]) || (new_angle > servo_max_angle[i]))
      {
        // 舵机角度值超出范围，忽略
        continue;
      }
      FWD.SetServoAngle(i, new_angle);  // 将最新的角度值，写入舵机
    }
    delay(20);
  }

  // 爪子舵机的开闭动作，不需要平滑过渡，直接设置角度给舵机
  FWD.SetServoAngle(FWD_ARM_SERVO_CLAW, action_to[FWD_ARM_SERVO_CLAW]); 
  delay(action_to[ARM_SERVOS]);
}

#ifdef USE_FWD
void updateServoAngle(int index, int servo, int invert)
{
  int angle = 0;
  if (joystick_value[index] > JOYSTICK_DIR_THRESHOLD)
  {
    angle = (JOYSTICK_CLAW_SERVO == index) ? CLAW_OPEN_ANGLE : (servo_cur_angle[servo] + JOYSTICK_DIR_P * invert);
  }
  else if (joystick_value[index] < -JOYSTICK_DIR_THRESHOLD)
  {
    angle = (JOYSTICK_CLAW_SERVO == index) ? CLAW_CLOSE_ANGLE : (servo_cur_angle[servo] + JOYSTICK_DIR_N * invert);
  }
  else
  {
    angle = servo_cur_angle[servo] + 0;
  }

  if ((angle <= servo_max_angle[servo]) &&
      (angle >= servo_min_angle[servo]))
  {
    FWD.SetServoAngle(servo, angle);
    servo_cur_angle[servo] = angle;
  }
}


void do_control(void)
{
  getJoystickValue(joystick_value);

  /*
  Joysitck 1: control ARM baseServo & LeftServo
  Joystick 2: contorl ARM rightServo & ClawServo
  */

  // for ARM Base Servo
  updateServoAngle(JOYSTICK_1_X, FWD_ARM_SERVO_BASE, -1);

  // for ARM Left Servo
  updateServoAngle(JOYSTICK_1_Y, FWD_ARM_SERVO_LEFT, 1);

  // for ARM Right Servo
  updateServoAngle(JOYSTICK_2_Y, FWD_ARM_SERVO_RIGHT, 1);

  // for ARM Claw Servo
  updateServoAngle(JOYSTICK_2_X, FWD_ARM_SERVO_CLAW, 1);


  // shall we open the FLASH LIGHT?
  if (getLedSwitchState())
  {
    FWD.setFlashLight(100);
  }
  else
  {
    FWD.setFlashLight(0);
  }

}

void init_FWD(void)
{
  FWD.init(MOTOR1, MOTOR2, MOTOR3, MOTOR4);
  FWD.SetControlMode(E_WIFI_CONTROL);

  FWD.InitServo();

  servo_min_angle[FWD_ARM_SERVO_BASE] = 0;   // 允许舵机旋转到达的最小值
  servo_max_angle[FWD_ARM_SERVO_BASE] = 180; // 允许舵机旋转到达的最大值
  servo_init_angle[FWD_ARM_SERVO_BASE] = picking_actions[COLOR_MIN][0][0]; // 刚上电时，舵机的初始角度

  servo_min_angle[FWD_ARM_SERVO_LEFT] = 10;  // 允许舵机旋转到达的最小值
  servo_max_angle[FWD_ARM_SERVO_LEFT] = 140; // 允许舵机旋转到达的最大值
  servo_init_angle[FWD_ARM_SERVO_LEFT] = picking_actions[COLOR_MIN][0][1]; // 刚上电时，舵机的初始角度

  servo_min_angle[FWD_ARM_SERVO_RIGHT] = 40;  // 允许舵机旋转到达的最小值
  servo_max_angle[FWD_ARM_SERVO_RIGHT] = 170; // 允许舵机旋转到达的最大值
  servo_init_angle[FWD_ARM_SERVO_RIGHT] = picking_actions[COLOR_MIN][0][2]; // 刚上电时，舵机的初始角度

  servo_min_angle[FWD_ARM_SERVO_CLAW] = CLAW_CLOSE_ANGLE;   // 允许舵机旋转到达的最小值
  servo_max_angle[FWD_ARM_SERVO_CLAW] = CLAW_OPEN_ANGLE;    // 允许舵机旋转到达的最大值
  servo_init_angle[FWD_ARM_SERVO_CLAW] = picking_actions[COLOR_MIN][0][3]; // 刚上电时，舵机的初始角度

  servo_min_angle[FWD_CAMERA_SERVO_YAW] = 0;   // 允许舵机旋转到达的最小值
  servo_max_angle[FWD_CAMERA_SERVO_YAW] = 180; // 允许舵机旋转到达的最大值
  servo_init_angle[FWD_CAMERA_SERVO_YAW] = 90; // 刚上电时，舵机的初始角度

  servo_min_angle[FWD_CAMERA_SERVO_PITCH] = 0;   // 允许舵机旋转到达的最小值
  servo_max_angle[FWD_CAMERA_SERVO_PITCH] = 180; // 允许舵机旋转到达的最大值
  servo_init_angle[FWD_CAMERA_SERVO_PITCH] = 90; // 刚上电时，舵机的初始角度

  // 给所有舵机设置初始角度
  for (int i = 0; i < FWD_SERVO_MAX; i++)
  {
    FWD.SetServoAngle(i, servo_init_angle[i]); // 写入舵机的初始角度
    servo_cur_angle[i] = servo_init_angle[i];
  }

  for (int i = 0; i < JOYSTICK_INDEX_MAX; i++)
  {
    joystick_value[i] = 0; // 摇杆值初始化为0
  }
}
#endif

void init_wifi(void)
{

#ifdef WORK_MODE_AP
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Camera Stream Ready! Connect to the ESP32 AP and go to: http://");
  Serial.println(IP);
#else
  // Wi-Fi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.println(WiFi.localIP());
#endif
}
