

#include "Somebot_MotorDriver.h"

//////////////////////Buzzer drive area///////////////////////////////////
//Buzzer initialization
void Buzzer_Setup(void)
{
  pinMode(PIN_BUZZER, OUTPUT);
  ledcSetup(BUZZER_CHN, BUZZER_FREQUENCY, 10);
  ledcAttachPin(PIN_BUZZER, BUZZER_CHN);
  ledcWriteTone(BUZZER_CHN, 0);
  delay(10);
}

//Buzzer alarm function
void Buzzer_Alert(int beat, int rebeat)
{
  beat = constrain(beat, 1, 9);
  rebeat = constrain(rebeat, 1, 255);
  for (int j = 0; j < rebeat; j++)
  {
    for (int i = 0; i < beat; i++)
    {
      ledcWriteTone(BUZZER_CHN, BUZZER_FREQUENCY);
      delay(100);
      ledcWriteTone(BUZZER_CHN, 0);
      delay(100);
    }
    delay(500);
  }
  ledcWriteTone(BUZZER_CHN, 0);
}

Somebot_MotorDriver::Somebot_MotorDriver(uint8_t version)
{
  mVersion = version;
}

void Somebot_MotorDriver::begin(int sdaPin, int sclPin, TwoWire &i2c_bus, uint8_t addr, uint16_t freq)
{
  mPCA9685Addr = addr;
  mPCA9685Freq = freq;

  // Creat object to represent PCA9685 at addr
  mPCA9685 = Adafruit_PWMServoDriver(mPCA9685Addr, i2c_bus);

  i2c_bus.setPins(sdaPin, sclPin);
  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode
  // some i2c devices dont like this so much so if you're sharing the bus, watch
  // out for this!
  // i2c_bus.setClock(400000);

  // Initialize PCA9685
  mPCA9685.begin();

  // Set PWM Frequency to 50Hz
  mPCA9685.setPWMFreq(mPCA9685Freq);

  for (uint8_t i = 0; i < 16; i++)
    mPCA9685.setPWM(i, 0, 0);
}

void Somebot_MotorDriver::setPWM(int pin, uint16_t value)
{
  if (value > 4095)
  {
    mPCA9685.setPWM(pin, 4096, 0);
  }
  else
    mPCA9685.setPWM(pin, 0, value);
}
void Somebot_MotorDriver::setPin(int pin, boolean value)
{
  if (value == LOW)
    mPCA9685.setPWM(pin, 0, 0);
  else
    mPCA9685.setPWM(pin, 4096, 0);
}

Somebot_DCMotor *Somebot_MotorDriver::getMotor(int num)
{
  if (num < MOTOR1 || num > MOTOR4)
    return NULL;

  if (mDCMotors[num].mMotorNum == MOTOR_NONE)
  {
    // not init'd yet!
    mDCMotors[num].mMotorNum = num;
    mDCMotors[num].MD = this;
    int pwm, in1, in2;
    if (num == MOTOR1)
    {
      if (mVersion == ESP32_DRIVER_BOARD_V1)
      {
        in1 = PCA9685_PWM_PIN_08;
        in2 = PCA9685_PWM_PIN_09;
      }
    }
    else if (num == MOTOR2)
    {
      if (mVersion == ESP32_DRIVER_BOARD_V1)
      {
        in1 = PCA9685_PWM_PIN_10;
        in2 = PCA9685_PWM_PIN_11;
      }
    }
    else if (num == MOTOR3)
    {
      if (mVersion == ESP32_DRIVER_BOARD_V1)
      {
        in1 = PCA9685_PWM_PIN_12;
        in2 = PCA9685_PWM_PIN_13;
      }
    }
    else if (num == MOTOR4)
    {
      if (mVersion == ESP32_DRIVER_BOARD_V1)
      {
        in1 = PCA9685_PWM_PIN_14;
        in2 = PCA9685_PWM_PIN_15;
      }
    }
    mDCMotors[num].mPWMPin = pwm;
    mDCMotors[num].mIn1Pin = in1;
    mDCMotors[num].mIn2Pin = in2;
  }

  return &mDCMotors[num];
}

Somebot_Servo *Somebot_MotorDriver::getServo(int num)
{
  if (num < SERVO1 || num > SERVO8)
    return NULL;

  if (mServos[num].mServoNum == SERVO_NONE)
  {
    // not init'd yet!
    mServos[num].mServoNum = num;
    mServos[num].MD = this;
    int pin;

    if (num == SERVO1)
    {
      if (mVersion == ESP32_DRIVER_BOARD_V1)
      {
        pin = PCA9685_PWM_PIN_00;
      }
    }
    else if (num == SERVO2)
    {
      if (mVersion == ESP32_DRIVER_BOARD_V1)
      {
        pin = PCA9685_PWM_PIN_01;
      }
    }
    else if (num == SERVO3)
    {
      if (mVersion == ESP32_DRIVER_BOARD_V1)
      {
        pin = PCA9685_PWM_PIN_02;
      }
    }
    else if (num == SERVO4)
    {
      if (mVersion == ESP32_DRIVER_BOARD_V1)
      {
        pin = PCA9685_PWM_PIN_03;
      }
    }
    else if (num == SERVO5)
    {
      if (mVersion == ESP32_DRIVER_BOARD_V1)
      {
        pin = PCA9685_PWM_PIN_04;
      }
    }
    else if (num == SERVO6)
    {
      if (mVersion == ESP32_DRIVER_BOARD_V1)
      {
        pin = PCA9685_PWM_PIN_05;
      }
    }
    else if (num == SERVO7)
    {
      if (mVersion == ESP32_DRIVER_BOARD_V1)
      {
        pin = PCA9685_PWM_PIN_06;
      }
    }
    else if (num == SERVO8)
    {
      if (mVersion == ESP32_DRIVER_BOARD_V1)
      {
        pin = PCA9685_PWM_PIN_07;
      }
    }

    mServos[num].mServoPin = pin;
  }
  return &mServos[num];
}

/******************************************
               SERVOS
******************************************/
Somebot_Servo::Somebot_Servo(void)
{
  MD = NULL;
  mServoNum = SERVO_NONE;
  mServoPin = 0;
  mCurrentAngle = 0;
  mOffsetAngle = 0;

  mPulseLength = 1000000;       // 1,000,000 us per second
  mPulseLength /= PCA9685_FREQ; // 50 Hz
  mPulseLength /= 4096;         // 12 bits of resolution
}

void Somebot_Servo::setServoPulse(double pulse)
{
  pulse *= 1000;
  pulse /= mPulseLength; // mPulseLength = 4.8828125
  if (MD != NULL)
  {
    MD->setPWM(mServoPin, pulse);
  }
}

void Somebot_Servo::setOffsetAngle(uint8_t angle)
{
  mOffsetAngle = angle;
}

void Somebot_Servo::writeAngle(uint8_t angle)
{
  mCurrentAngle = mOffsetAngle + angle;
  if (mCurrentAngle < 0)
  {
    mCurrentAngle = 0;
  }
  else if (mCurrentAngle > 180)
  {
    mCurrentAngle = 180;
  }

  double pulse = 0.5 + mCurrentAngle / 90.0;
  setServoPulse(pulse);
}

uint8_t Somebot_Servo::readAngle()
{
  return mCurrentAngle;
}

/******************************************
               MOTORS
******************************************/
Somebot_DCMotor::Somebot_DCMotor(void)
{
  MD = NULL;
  mMotorNum = MOTOR_NONE;
  mPWMPin = mIn1Pin = mIn2Pin = 0;
}

void Somebot_DCMotor::run(int cmd)
{
#ifdef MOTORDEBUG
#endif
  if (MD == NULL)
  {
    return;
  }

  switch (cmd)
  {
  case MOTOR_FORWARD:
    if (MD->mVersion == ESP32_DRIVER_BOARD_V1)
    {
      MD->setPin(mIn2Pin, LOW);
      MD->setPWM(mIn1Pin, mDcSpeed);
    }
    break;
  case MOTOR_BACKWARD:
    if (MD->mVersion == ESP32_DRIVER_BOARD_V1)
    {
      MD->setPin(mIn1Pin, LOW);
      MD->setPWM(mIn2Pin, mDcSpeed);
    }
    break;
  case MOTOR_BRAKE:
    if (MD->mVersion == ESP32_DRIVER_BOARD_V1)
    {
      MD->setPin(mIn1Pin, HIGH);
      MD->setPin(mIn2Pin, HIGH);
    }
    break;
  case MOTOR_RELEASE:
    if (MD->mVersion == ESP32_DRIVER_BOARD_V1)
    {
      MD->setPin(mIn1Pin, LOW);
      MD->setPin(mIn2Pin, LOW);
    }
    break;
  }
}

void Somebot_DCMotor::setSpeed(uint8_t speed)
{
  if (MD->mVersion == ESP32_DRIVER_BOARD_V1)
  {
    mDcSpeed = (speed * 16);
  }
}

/******************************************
               SENSOR
******************************************/
Somebot_Sensor::Somebot_Sensor(void)
{
  MD = NULL;
  mNotifyLed = NULL;
  mFlashLight = NULL;
  mNotifyLedPin = mFlashLightPin = mUsEchoPin = mUsTrigPin = mBuzzerPin = 0;
}

uint16_t Somebot_Sensor::GetUltrasonicDistance(void)
{
  uint16_t FrontDistance;
  digitalWrite(mUsTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(mUsTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(mUsTrigPin, LOW);
  FrontDistance = pulseIn(mUsEchoPin, HIGH) / 58.00;
  return FrontDistance;
}

void Somebot_MotorDriver::initSensors(void)
{
  if (mSensors.mNotifyLed == NULL)
  {
    mSensors.mNotifyLedPin = ESP32_CAM_NOTIFY_LED_PIN;
    // pinMode(mNotifyLedPin, OUTPUT);
    mSensors.mNotifyLed = new Esp32Led();
    mSensors.mNotifyLed->init(mSensors.mNotifyLedPin);
    mSensors.mNotifyLed->write(LED_OFF);
  }

  if (mSensors.mFlashLight == NULL)
  {
    mSensors.mFlashLightPin = ESP32_CAM_FLASHLIGHT_PIN;
    // pinMode(mFlashLightPin, OUTPUT);
    mSensors.mFlashLight = new Esp32Led();
    mSensors.mFlashLight->init(mSensors.mFlashLightPin);
    mSensors.mFlashLight->write(LED_OFF);
  }

  mSensors.mUsEchoPin = US_ECHO_PIN;
  mSensors.mUsTrigPin = US_TRIG_PIN;
  pinMode(mSensors.mUsEchoPin, INPUT);
  pinMode(mSensors.mUsTrigPin, OUTPUT);
}

void *Somebot_MotorDriver::getSensor(int n)
{
  if (n == SENSOR_NOTIFY_LED)
  {
    if (mSensors.mNotifyLed == NULL)
    {
      mSensors.mNotifyLedPin = ESP32_CAM_NOTIFY_LED_PIN;
      // pinMode(mNotifyLedPin, OUTPUT);
      mSensors.mNotifyLed = new Esp32Led();
      mSensors.mNotifyLed->init(mSensors.mNotifyLedPin);
    }
    return mSensors.mNotifyLed;
  }
  else if (n == SENSOR_FLASH_LIGHT)
  {
    if (mSensors.mFlashLight == NULL)
    {
      mSensors.mFlashLightPin = ESP32_CAM_FLASHLIGHT_PIN;
      // pinMode(mFlashLightPin, OUTPUT);
      mSensors.mFlashLight = new Esp32Led();
      mSensors.mFlashLight->init(mSensors.mFlashLightPin);
    }
    return mSensors.mFlashLight;
  }
  else if (n == SENSOR_ULTRASONIC)
  {
    mSensors.mUsEchoPin = US_ECHO_PIN;
    mSensors.mUsTrigPin = US_TRIG_PIN;
    pinMode(mSensors.mUsEchoPin, INPUT);
    pinMode(mSensors.mUsTrigPin, OUTPUT);
    return NULL;
  }

  return NULL;
}

Somebot_Sensor *Somebot_MotorDriver::getSensors()
{
  initSensors();

  mSensors.MD = this;
  return &mSensors;
}
