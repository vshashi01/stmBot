#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

namespace stmbot
{
	void setupUltrasonicSensorSystem(void);
} // namespace stmbot

extern "C" void EXTI9_5_IRQHandler(void);
extern "C" void TIM3_IRQHandler(void);

#endif // !ULTRASONIC_SENSOR_H