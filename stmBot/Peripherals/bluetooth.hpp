#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "qpcpp.h"
#include "transmitUART/uart_transmit.hpp"

namespace stmbot
{
	extern stmapp::UARTTransmitProcess BluetoothTransmit;
	bool RequestBluetoothTransmit(const char *string, const size_t length);

	typedef QF_MPOOL_EL(stmapp::UartTransmitEvt) BluetoothTransmitPool;
	typedef stmapp::UartTransmitEvt BluetoothTransmitEvt;
} // namespace stmbot

extern "C" void USART2_IRQHandler(void);

#endif // !"BLUETOOTH_H"