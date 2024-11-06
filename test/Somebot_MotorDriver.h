#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "esp32_cam_gpio_config.h"
#include "Adafruit_PWMServoDriver.h"
#include "Esp32Led.h"

#ifndef SOMEBOT_ESP32_DRIVER_BOARD_H
#define SOMEBOT_ESP32_DRIVER_BOARD_H

//#define MOTORDEBUG

#define ESP32_DRIVER_BOARD_V1 1

#define PCA9685_ADDR (0x40)
#define PCA9685_WIRE (Wire)
#define PCA9685_FREQ (50)

enum
{
	PCA9685_PWM_PIN_00 = 0,
	PCA9685_PWM_PIN_01,
	PCA9685_PWM_PIN_02,
	PCA9685_PWM_PIN_03,
	PCA9685_PWM_PIN_04,
	PCA9685_PWM_PIN_05,
	PCA9685_PWM_PIN_06,
	PCA9685_PWM_PIN_07,
	PCA9685_PWM_PIN_08,
	PCA9685_PWM_PIN_09,
	PCA9685_PWM_PIN_10,
	PCA9685_PWM_PIN_11,
	PCA9685_PWM_PIN_12,
	PCA9685_PWM_PIN_13,
	PCA9685_PWM_PIN_14,
	PCA9685_PWM_PIN_15,
};

enum
{
  SERVO1 = 0,
  SERVO2,
  SERVO3,
  SERVO4,
  SERVO5,
  SERVO6,
  SERVO7,
  SERVO8,
  SERVO_NONE
};

enum
{
  MOTOR1 = 0,
  MOTOR2,
  MOTOR3,
  MOTOR4,
  MOTOR_NONE
};

enum
{
  MOTOR_FORWARD = 0,
  MOTOR_BACKWARD,
  MOTOR_BRAKE,
  MOTOR_RELEASE,
};

enum
{
  SENSOR_NOTIFY_LED = 0,
  SENSOR_FLASH_LIGHT,
  SENSOR_ULTRASONIC,
  SENSOR_BUZZEER,
  SENSOR_MAX,
};

//////////////////////Buzzer drive area///////////////////////////////////
//Buzzer pin definition             
#define PIN_BUZZER GPIO_02                    //Define the pins for the ESP32 control buzzer
#define BUZZER_CHN 15                    //Define the PWM channel for ESP32
#define BUZZER_FREQUENCY 2000           //Define the resonant frequency of the buzzer 

void Buzzer_Setup(void);                //Buzzer initialization
void Buzzer_Alert(int beat, int rebeat);//Buzzer alarm function


class Somebot_MotorDriver;

class Somebot_Sensor
{
public:
  Somebot_Sensor(void);  
  friend class Somebot_MotorDriver;
  // Buzzer *mBuzzer;
  Esp32Led *mNotifyLed;
  Esp32Led *mFlashLight;
  uint16_t GetUltrasonicDistance(void);

private:
  uint8_t mNotifyLedPin;
  uint8_t mFlashLightPin;
  uint8_t mUsEchoPin, mUsTrigPin;
  uint8_t mBuzzerPin;
  Somebot_MotorDriver *MD;
};

class Somebot_DCMotor
{
public:
  Somebot_DCMotor(void);
  friend class Somebot_MotorDriver;
  void run(int);
  void setSpeed(uint8_t);

private:
  int mPWMPin, mIn1Pin, mIn2Pin;
  int mDcSpeed;
  uint8_t mMotorNum;
  Somebot_MotorDriver *MD;
};

class Somebot_Servo
{
public:
  Somebot_Servo(void);
  friend class Somebot_MotorDriver;
  void setServoPulse(double pulse);
  void setOffsetAngle(uint8_t angle);
  void writeAngle(uint8_t angle);
  uint8_t readAngle();

private:
  int mServoPin;
  uint8_t mServoNum, mOffsetAngle, mCurrentAngle;
  double mPulseLength;
  Somebot_MotorDriver *MD;
};

class Somebot_MotorDriver
{
public:
  Somebot_MotorDriver(uint8_t version = ESP32_DRIVER_BOARD_V1);
  uint8_t mVersion;

  void begin(int sdaPin = PCA9685_SDA_PIN, int sclPin = PCA9685_SCL_PIN,
             TwoWire &i2c_bus = PCA9685_WIRE, uint8_t addr = PCA9685_ADDR,
             uint16_t freq = PCA9685_FREQ);

  void setPWM(int pin, uint16_t val);
  void setPin(int pin, boolean val);
  Somebot_DCMotor *getMotor(int n);
  Somebot_Servo *getServo(int n);
  void initSensors(void);
  void *getSensor(int n);
  Somebot_Sensor *getSensors();

private:
  uint8_t mPCA9685Addr;
  uint16_t mPCA9685Freq;
  Adafruit_PWMServoDriver mPCA9685;
  Somebot_DCMotor mDCMotors[4];
  Somebot_Servo mServos[8];
  Somebot_Sensor mSensors;
};

#endif
