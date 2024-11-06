
#include <Arduino.h>
#include "SmartCar.h"
#include "Somebot_MotorDriver.h"

#ifndef _FourWheelDrive_H_
#define _FourWheelDrive_H_

enum
{
  FWD_ARM_SERVO_BASE = 0,
  FWD_ARM_SERVO_LEFT,
  FWD_ARM_SERVO_RIGHT,
  FWD_ARM_SERVO_CLAW,
  FWD_CAMERA_SERVO_YAW,
  FWD_CAMERA_SERVO_PITCH,
  FWD_SERVO_MAX
};

enum
{
  FWD_LEFT_FOWARD_MOTOR = 0,
  FWD_RIGHT_FORWARD_MOTOR,
  FWD_LEFT_BACKWARD_MOTOR,
  FWD_RIGHT_BACKWARD_MOTOR,
  FWD_MOTOR_MAX
};

class FourWheelDrive : public SmartCar
{

private:
  void DriveSpeed(uint8_t s);

public:
  Somebot_MotorDriver mMotorDriver;
  Somebot_DCMotor *mDcMotors[FWD_MOTOR_MAX];
  Somebot_Servo *mServos[FWD_SERVO_MAX];
  Somebot_Sensor *mSensors;

  FourWheelDrive();
  ~FourWheelDrive(void);

  void init(int leftForward = MOTOR1, int rightForward = MOTOR2, int leftBackward = MOTOR3, int rightBackward = MOTOR4);

  uint16_t GetUltrasonicValue();
  void setFlashLight(int brightness);

  void InitServo(void);
  void SetServoOffsetAngle(int servo, uint8_t angle);
  void SetServoAngle(int servo, uint8_t angle);
};

#endif /* _AURORARACING_H_ */
