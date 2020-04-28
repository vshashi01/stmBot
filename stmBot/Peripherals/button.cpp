#include "button.hpp"
#include "app_signals.h"

namespace
{
	const auto buttonReleasedEvent = QP::QEvt{
		stmapp::castToQPSignal(STMAPP_SIGNALS::LED_RESTART_TIMER_SIGNAL), 0, 0};

	const auto buttonPressedEvent =
		QP::QEvt{stmapp::castToQPSignal(STMAPP_SIGNALS::LED_ON_SIGNAL), 0, 0};

	constexpr auto buttonStruct =
		stmapp::PushButtonStruct(GPIO_INSTANCE::GPIO_A, PIN::PIN_4, 25,
								 &buttonReleasedEvent, &buttonPressedEvent);
} // namespace

stmapp::PushButton stmbot::Button1(&buttonStruct);