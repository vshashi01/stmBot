#include "bluetooth.hpp"
#include "app_signals.h"
#include "etl/cstring.h"
#include "isr_priority.h"
#include <cstring>

namespace
{
	void process_message_dma(const etl::istring &message)
	{
		etl::string<UART_BUFFER_SIZE> temp;
		temp.append(message);
		temp.append("+OK");
		stmbot::RequestBluetoothTransmit(temp.data(), temp.size());
	}

	void transmit_complete()
	{
		auto complete_event = Q_NEW(
			QP::QEvt,
			stmapp::castToQPSignal(STMAPP_SIGNALS::UART_TRANSMIT_COMPLETE_SIGNAL));

		stmbot::BluetoothTransmit.POST(complete_event, &UartDMA);
	}

	auto transmit_buffer = etl::string<UART_BUFFER_SIZE>();
	auto receive_buffer  = etl::string<UART_BUFFER_SIZE>();

	constexpr stmcpp::UARTInterruptDMA UartDMA = stmcpp::UARTInterruptDMA(
		USART_INSTANCE::USART_2, 115200, stmbot::BT_PRIO, &transmit_buffer,
		&receive_buffer, &process_message_dma, &transmit_complete);
} // namespace

stmapp::UARTTransmitProcess
	stmbot::BluetoothTransmit(STMAPP_SIGNALS::START_TRANSMIT_UART_SIGNAL,
							  STMAPP_SIGNALS::UART_TRANSMIT_COMPLETE_SIGNAL,
							  &UartDMA);

bool stmbot::RequestBluetoothTransmit(const char *string, const size_t length)
{
	// ensure the transferred length can be handled by the buffer
	const auto transfer_length_ =
		length > UART_BUFFER_SIZE ? UART_BUFFER_SIZE : length;

	if (transfer_length_ > 0)
	{
		auto event = Q_NEW(
			BluetoothTransmitEvt,
			stmapp::castToQPSignal(STMAPP_SIGNALS::START_TRANSMIT_UART_SIGNAL));

		std::memcpy(&(event->transmit_string_[0]), string, transfer_length_);

		event->length = transfer_length_;
		return BluetoothTransmit.POST(event, &UARTDMA);
	}
	return false;
}

void USART2_IRQHandler()
{
	// interrupt service
	UartDMA.serviceUARTInterrupt();
}