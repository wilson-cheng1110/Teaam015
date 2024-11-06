#include <Arduino.h>


#ifndef _SMART_CAR_H_
#define _SMART_CAR_H_

typedef enum
{
    E_SMARTCAR = 0,
    E_HUMMER_BOT,
    E_AURORA_RACING,
    E_PANTHER_TANK,
    E_BEETLE_BOT,
    E_BALANCE_CAR,
    E_OTTO,
    E_QUADCOPTER,
    E_HELLO_BOT,
    E_MIRAGE_TANK,
    E_FOUR_WHEEL_DRIVE,
    E_MECANUMCAR,
    E_TYPE_MAX,
} robot_type_t;

typedef enum
{
    E_WIFI_CONTROL = 0,
    E_BLUETOOTH_CONTROL,
    E_INFRARED_REMOTE_CONTROL,
    E_INFRARED_TRACKING_MODE,
    E_INFRARED_AVOIDANCE_MODE,
    E_ULTRASONIC_AVOIDANCE,
    E_PS2_REMOTE_CONTROL,
    E_NRF24L01_CONTROL,
    E_ULTRASONIC_INFRARED_AVOIDANCE,
    E_PIANO_MODE,
    E_RGB_MODE,
    E_LED_MAXTRIX_MODE,
    E_CMD_LINE_MODE,
    E_LIGHT_SEEKING_MODE,
    E_ULTRASONIC_FOLLOW_MODE,
    E_CTRL_MODE_MAX,
} robot_ctrl_mode_t;

typedef enum
{
    E_FORWARD = 0,
    E_BACK,
    E_LEFT,
    E_RIGHT,
    E_RIGHT_ROTATE,
    E_LEFT_ROTATE,
    E_STOP,
    E_RUNNING,
    E_SPEED_UP,
    E_SPEED_DOWN,
    E_LOW_POWER,
} robot_status_t;

class SmartCar {
private :
    String mSmartCarName;
    robot_type_t mSmartCarType;
    byte mSmartCarAddr;
    robot_ctrl_mode_t mControlMode;
    robot_status_t mStatus;
public :
    SmartCar(String name, robot_type_t type, byte addr, robot_ctrl_mode_t control_mode = E_WIFI_CONTROL);
    ~SmartCar();
    uint8_t mBatteryValue;
    uint8_t mTemperature;
    uint8_t mSpeed ;
    int mDegree;
    void Move(int directions);
    void GoForward(void);
    void GoBack(void);
    void TurnLeft(void);
    void TurnRight(void);
    void KeepStop(void);
    void Drive(int degree);
    void SetSpeed(uint8_t s);
    void SpeedUp(uint8_t Duration = 5);
    void SpeedDown(uint8_t Duration = 5);
    uint8_t  GetSpeed(void);
    void SetControlMode(robot_ctrl_mode_t mode);
    void SetStatus(robot_status_t status);
    robot_ctrl_mode_t GetControlMode(void);
    robot_status_t GetStatus(void);
    uint8_t GetBattery(void);
    uint8_t GetTemperature(void);
};
#endif  // _SMART_CAR_H_
