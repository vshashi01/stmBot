#pragma once

#include "stm_app.h"

enum class STMAPP_SIGNALS : QP::QSignal
{
	NULL_SIGNALS = stmapp::APP_START_SIGNAL,
	LED_ON_SIGNAL,
	LED_OFF_SIGNAL,
	LED_RESTART_TIMER_SIGNAL,
	START_TRANSMIT_UART_SIGNAL,
	UART_TRANSMIT_COMPLETE_SIGNAL,
};
