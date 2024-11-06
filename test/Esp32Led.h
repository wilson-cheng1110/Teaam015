#include <Arduino.h>
#include <Servo.h>

#ifndef ESP32_LED
#define ESP32_LED

// be carefull: LED and Servo share 16 pwm channels

#define LED_MIN_BRIGHTNESS 0
#define LED_MAX_BRIGHTNESS 100
#define LED_OFF LED_MIN_BRIGHTNESS
#define LED_ON  LED_MAX_BRIGHTNESS

#define LED_TIMER_BIT_WIDTH 16
#define LED_MAX_COMPARE ((1 << LED_TIMER_BIT_WIDTH) - 1) // 255

#define LED_MIN_PULSE_WIDTH 0     // the shortest pulse sent to a LED
#define LED_MAX_PULSE_WIDTH ((1<<16)-1) // 65535 // the longest pulse sent to a LED

class Esp32Led : public Servo
{
private:
  int mBrightness;

public:
  bool init(uint8_t pin);
  void setBrightness(int brightness);
};

#endif
