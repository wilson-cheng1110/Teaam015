
#include "Esp32Led.h"

bool Esp32Led::init(uint8_t pin)
{
    return this->attach(pin, CHANNEL_NOT_ATTACHED, LED_MIN_BRIGHTNESS, LED_MAX_BRIGHTNESS, LED_MIN_PULSE_WIDTH, LED_MAX_PULSE_WIDTH);
}

void Esp32Led::setBrightness(int brightness)
{
    if (brightness < LED_MIN_BRIGHTNESS)
    {
        brightness = LED_MIN_BRIGHTNESS;
    }
    else if (brightness > LED_MAX_BRIGHTNESS)
    {
        brightness = LED_MAX_BRIGHTNESS;
    }
    mBrightness = brightness;

    this->write(mBrightness);
}
