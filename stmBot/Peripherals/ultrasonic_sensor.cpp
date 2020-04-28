#include "ultrasonic_sensor.hpp"
#include "isr_priority.h"

#include "stmEXTI/stm_exti.h"
#include "stmIO/stm_io.h"
#include "stmTimer/stm_timer.h"

namespace
{
	void echo_callback(void);
	void ultrasonic_trigger_callback(void);

	/* Testing the ultrasonic sensor with interrupts and timer interrupts */
	constexpr auto ultrasonic_sensor_input = stmcpp::DigitalIn(
		GPIO_INSTANCE::GPIO_A, PIN::PIN_7, PIN_INPUT_TYPE::FLOATING_INPUT,
		PIN_PULL_TYPE::PULLDOWN_INPUT);

	constexpr stmcpp::DigitalOut toggle_led =
		stmcpp::DigitalOut(GPIO_INSTANCE::GPIO_B, PIN::PIN_10,
						   PIN_OUTPUT_SPEED::STD_OUTPUT, PIN_OUTPUT_TYPE::PUSH_PULL);

	// constexpr auto echo_interrupt =
	// stmcpp::ExternalInterrupt::constructInterruptOnRise( EXTI_LINE_0_4::LINE_0,
	// ECHO_RECEIVE_PRIO, &ultrasonic_sensor_input, &echo_callback);

	constexpr auto interrupt_config =
		stmcpp::CollectionExternalInterrupt<EXTI_LINE_5_9>::constructInterruptConfig(
			EXTI_LINE_5_9::LINE_7, &ultrasonic_sensor_input, true, false,
			&echo_callback);

	constexpr auto echo_interrupt = stmcpp::CollectionExternalInterrupt<
		EXTI_LINE_5_9>::constructCollectionInterrupt(stmbot ::ECHO_RECEIVE_PRIO,
													 &interrupt_config);

	// const stmcpp::ExternalInterrupt *echo_interrupt_ptr = &echo_interrupt;
	// const stmcpp::CollectionExternalInterrupt<EXTI_LINE_5_9> *echo_interrupt_ptr =
	//&echo_interrupt;

	constexpr auto ultrasonic_trigger_pin =
		stmcpp::DigitalOut(GPIO_INSTANCE::GPIO_B, PIN::PIN_15,
						   PIN_OUTPUT_SPEED::STD_OUTPUT, PIN_OUTPUT_TYPE::PUSH_PULL);

	// const stmcpp::DigitalOut *ultrasonic_trigger_pin_ptr =
	// &ultrasonic_trigger_pin;

	constexpr auto ultrasonic_trigger_timer =
		stmcpp::TimeBaseInterruptGenerator::constructTimerBaseGenerator(
			GENERAL_TIMER_INSTANCE::TIM_3, TIMER_ALIGNMENT::EDGE_ALIGNED, true, 2249,
			63999, stmbot::TIM3_PRIO, &ultrasonic_trigger_callback);

	// const stmcpp::TimeBaseInterruptGenerator *ultrasonic_trigger_timer_ptr =
	//&ultrasonic_trigger_timer;
	/* Testing the ultrasonic sensor with interrupts and timer interrupts */

	void echo_callback()
	{
		// empty
		static auto counter = 0;
		if (counter > 1)
		{
			// Blink2->postLIFO(
			// Q_NEW(QP::QEvt,
			// stmapp::castToQPSignal(STMAPP_SIGNALS::LED_ON_SIGNAL)));
			// toggle_led_ptr->toggle();
			toggle_led.toggle();
			counter = 0;
		}
		else
		{
			counter++;
		}
	}

	void ultrasonic_trigger_callback(void)
	{
		toggle_led.toggle();
		ultrasonic_trigger_pin.high();
		delay_us(10);
		ultrasonic_trigger_pin.low();
	}
} // namespace

void stmbot::setupUltrasonicSensorSystem()
{
	ultrasonic_trigger_pin.setup();
	ultrasonic_trigger_timer.setup();
	ultrasonic_trigger_timer.enable();
	echo_interrupt.setup();
	echo_interrupt.enable();
	toggle_led.setup();
	toggle_led.low();
}

void EXTI9_5_IRQHandler(void) { echo_interrupt.serviceInterrupt(); }

void TIM3_IRQHandler(void) { ultrasonic_trigger_timer.serviceInterrupt(); }