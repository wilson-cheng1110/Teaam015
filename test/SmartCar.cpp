#include "SmartCar.h"


SmartCar::SmartCar(String name, robot_type_t type, byte addr, robot_ctrl_mode_t control_mode)
{
    mSmartCarName = name;
    mSmartCarType = type;
    mSmartCarAddr = addr;
    mControlMode = control_mode;
    mStatus = E_STOP;
    mBatteryValue = 0;
    mTemperature = 0;
    mSpeed = 0 ;
    mDegree = 0;
}

SmartCar::~SmartCar(void)
{

}

void SmartCar::SetControlMode(robot_ctrl_mode_t mode)
{
    if (mode < E_CTRL_MODE_MAX && mode >= 0)
    mControlMode = mode;
}

robot_ctrl_mode_t SmartCar::GetControlMode(void)
{
    return mControlMode;
}

void SmartCar::SetSpeed(uint8_t s)
{
    // DEBUG_LOG(DEBUG_LEVEL_INFO, "SetSpeed =%d \n", s);
    if (s > 100) {
        mSpeed = 100;
        return;
    } else if (s < 0) {
        mSpeed = 0;
        return;
    }
    mSpeed = s;
}

uint8_t SmartCar::GetSpeed(void)
{
    return mSpeed;
}


void SmartCar::SpeedUp(uint8_t Duration)
{
    SetSpeed(mSpeed + Duration);
    mStatus = E_SPEED_UP;
}

void SmartCar::SpeedDown(uint8_t Duration )
{
    SetSpeed(mSpeed - Duration);
    mStatus = E_SPEED_DOWN;
}

void SmartCar::SetStatus(robot_status_t status)
{
    mStatus = status;
}

robot_status_t SmartCar::GetStatus(void)
{
    return mStatus;
}

uint8_t SmartCar::GetBattery(void)
{
    return mBatteryValue;
}

uint8_t SmartCar::GetTemperature(void)
{
    return mTemperature;
}
