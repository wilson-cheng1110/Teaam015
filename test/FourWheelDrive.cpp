
#include <math.h>
#include "FourWheelDrive.h"

#define MECANUMCAR (0)

FourWheelDrive::FourWheelDrive() : SmartCar("fwd", E_FOUR_WHEEL_DRIVE, 0x00, E_WIFI_CONTROL)
{
  mSensors = NULL;
  for (int i = 0; i < FWD_SERVO_MAX; i++)
  {
    mServos[i] = NULL;
  }

  for (int i = 0; i < FWD_MOTOR_MAX; i++)
  {
    mDcMotors[i] = NULL;
  }

  SetStatus(E_STOP);
}

FourWheelDrive::~FourWheelDrive()
{
  for (int i = 0; i < FWD_SERVO_MAX; i++)
  {
    delete mServos[i];
  }

  for (int i = 0; i < FWD_MOTOR_MAX; i++)
  {
    delete mDcMotors[i];
  }
}

void FourWheelDrive::init(int leftForward, int rightForward, int leftBackward, int rightBackward)
{
  mMotorDriver = Somebot_MotorDriver();
  mMotorDriver.begin();

  mSensors = mMotorDriver.getSensors();
  mDcMotors[FWD_LEFT_FOWARD_MOTOR] = mMotorDriver.getMotor(leftForward);
  mDcMotors[FWD_RIGHT_FORWARD_MOTOR] = mMotorDriver.getMotor(rightForward);
  mDcMotors[FWD_LEFT_BACKWARD_MOTOR] = mMotorDriver.getMotor(leftBackward);
  mDcMotors[FWD_RIGHT_BACKWARD_MOTOR] = mMotorDriver.getMotor(rightBackward);

  Buzzer_Setup();    //Buzzer initialization function
  Buzzer_Alert(4, 3);//Control the buzzer to sound 3 times, 4 sounds each time
}

void FourWheelDrive::DriveSpeed(uint8_t s)
{
  if (s >= 0 && s <= 100)
  {
    mDcMotors[FWD_LEFT_FOWARD_MOTOR]->setSpeed((s / 10) * 25.5);
    mDcMotors[FWD_RIGHT_FORWARD_MOTOR]->setSpeed((s / 10) * 25.5);
    mDcMotors[FWD_LEFT_BACKWARD_MOTOR]->setSpeed((s / 10) * 25.5);
    mDcMotors[FWD_RIGHT_BACKWARD_MOTOR]->setSpeed((s / 10) * 25.5);
  }
}


// ---------------------------------------  sensors --------------------------------------- //
uint16_t FourWheelDrive::GetUltrasonicValue()
{
  return mSensors->GetUltrasonicDistance();
}

void FourWheelDrive::setFlashLight(int brightness)
{
  mSensors->mFlashLight->write(brightness);
}

// --------------------------------------- Servo --------------------------------------- //
void FourWheelDrive::InitServo(void)
{
  mServos[FWD_ARM_SERVO_BASE] = mMotorDriver.getServo(SERVO1);
  mServos[FWD_ARM_SERVO_LEFT] = mMotorDriver.getServo(SERVO2);
  mServos[FWD_ARM_SERVO_RIGHT] = mMotorDriver.getServo(SERVO3);
  mServos[FWD_ARM_SERVO_CLAW] = mMotorDriver.getServo(SERVO4);

  mServos[FWD_CAMERA_SERVO_YAW] = mMotorDriver.getServo(SERVO5);
  mServos[FWD_CAMERA_SERVO_PITCH] = mMotorDriver.getServo(SERVO6);
}

void FourWheelDrive::SetServoOffsetAngle(int servo, uint8_t angle)
{
  if (servo < FWD_ARM_SERVO_BASE || servo > FWD_CAMERA_SERVO_PITCH)
  {
    return;
  }

  mServos[servo]->setOffsetAngle(angle);
}

void FourWheelDrive::SetServoAngle(int servo, uint8_t angle)
{
  if (servo < FWD_ARM_SERVO_BASE || servo > FWD_CAMERA_SERVO_PITCH)
  {
    return;
  }

  mServos[servo]->writeAngle(angle);
}
