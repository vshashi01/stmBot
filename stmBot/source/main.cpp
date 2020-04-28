#include "qpcpp.hpp"

#include "etl/cstring.h"

#include "bsp.h"

#include "Peripherals/blinking_led.hpp"
#include "Peripherals/bluetooth.hpp"
#include "Peripherals/button.hpp"
#include "Peripherals/ultrasonic_sensor.hpp"
#include "blinkLED/blink_led.h"
#include "pushButton/push_button.h"
#include "stmEXTI/stm_exti.h"
#include "stmPin/stm_pin.h"
#include "stmTimer/stm_timer.h"
#include "stmUSART/stm_usart.h"

volatile uint32_t sysTicks = 0;

enum AO_PRIORITY : uint8_t
{
	UART_TRANSMIT = 1U,
	BLINK_LED_WITH_TIMER,
	STATIC_LED,
	PUSH_BUTTON,
};

int main(void)
{
	/* Active objects from stmAPP */
	QP::QActive *const Blink1		 = &stmbot::LED1;
	QP::QActive *const Blink2		 = &stmbot::LED2;
	QP::QActive *const PushButton1   = &stmbot::Button1;
	QP::QActive *const uart_transmit = &stmbot::BluetoothTransmit;

	BSP::initBoard();

	/* Active object event queues */
	static QP::QEvt const *led1QueueStore[5];
	static QP::QEvt const *led2QueueStore[5];
	static QP::QEvt const *button1QueueStore[5];
	static QP::QEvt const *bluetoothQueueStore[5];

	/* Subscriber list array */
	static QP::QSubscrList subscribeTo[20];

	/** Event pool array definition */
	static QF_MPOOL_EL(QP::QEvt) buttonPoolStore[10]; // small pool
	static stmbot::BluetoothTransmitPool uartPoolStore[5]; // medium pool

	/* initialize the framework and the Vanilla scheduler 'qv' */
	QP::QF::init();

	/* initialize the publish subscribe facility */
	QP::QF::psInit(subscribeTo, Q_DIM(subscribeTo));

	/* Intialize event pool arrays*/
	QP::QF::poolInit(buttonPoolStore, sizeof(buttonPoolStore),
					 sizeof(buttonPoolStore[0]));
	QP::QF::poolInit(uartPoolStore, sizeof(uartPoolStore), sizeof(uartPoolStore[0]));

	BSP::setupClock();

	/* Active objects start method */
	Blink1->start(AO_PRIORITY::BLINK_LED_WITH_TIMER, &led1QueueStore[0],
				  Q_DIM(led1QueueStore), (void *)0, 0U);
	Blink2->start(AO_PRIORITY::STATIC_LED, &led2QueueStore[0], Q_DIM(led2QueueStore),
				  (void *)0, 0U);
	PushButton1->start(AO_PRIORITY::PUSH_BUTTON, &button1QueueStore[0],
					   Q_DIM(button1QueueStore), (void *)0, 0U);
	uart_transmit->start(AO_PRIORITY::UART_TRANSMIT, &bluetoothQueueStore[0],
						 Q_DIM(bluetoothQueueStore), (void *)0, 0U);
	/* Active objects start method */

	return QP::QF::run();
}

void QP::QF::onStartup(void)
{
	BSP::enableSysTickHandler();
	stmbot::setupUltrasonicSensorSystem();
}

void QP::QF::onCleanup() {}

void QP::QV::onIdle() { QF_INT_ENABLE(); }

extern "C" void Q_onAssert(char const *module, int loc)
{
	(void)module;
	(void)loc;
	QS_ASSERTION(module, loc, static_cast<uint32_t>(10000U));
	// NVIC_SystemReset ();
}