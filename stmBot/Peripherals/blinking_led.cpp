#include "blinking_led.hpp"
#include "app_signals.h"

namespace
{
	constexpr auto blinkLed1Struct =
		stmapp::BlinkLEDStruct(GPIO_INSTANCE::GPIO_C, PIN::PIN_13, 500, 700);

	constexpr auto blinkLed2Struct = stmapp::BlinkLEDStruct(
		GPIO_INSTANCE::GPIO_C, PIN::PIN_15, false, 300, 300,
		STMAPP_SIGNALS::LED_ON_SIGNAL, STMAPP_SIGNALS::LED_OFF_SIGNAL,
		STMAPP_SIGNALS::LED_RESTART_TIMER_SIGNAL); // namespace
} // namespace

stmapp::BlinkLED stmbot::LED1(&blinkLed1Struct);
stmapp::BlinkLED stmbot::LED2(&blinkLed2Struct);