#pragma once

#include "qpcpp.h"
#include "stm32f103xb.h"

namespace stmbot
{
	enum KernelAwareISRs
	{
		SYSTICK_PRIO = QF_AWARE_ISR_CMSIS_PRI,
		TIM3_PRIO,
		ECHO_RECEIVE_PRIO,
		BT_PRIO,
		// see NOTE00
		// ...
		MAX_KERNEL_AWARE_CMSIS_PRI // keep always last
	};

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!! CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// Assign a priority to EVERY ISR explicitly by calling NVIC_SetPriority().
	// DO NOT LEAVE THE ISR PRIORITIES AT THE DEFAULT VALUE!
	//
	enum KernelUnawareISRs
	{
		// see NOTE00
		// ...
		MAX_KERNEL_UNAWARE_CMSIS_PRI // keep always last
	};
	// "kernel-unaware" interrupts can't overlap "kernel-aware" interrupts
	Q_ASSERT_COMPILE(MAX_KERNEL_UNAWARE_CMSIS_PRI <= QF_AWARE_ISR_CMSIS_PRI);

	// "kernel-aware" interrupts should not overlap the PendSV priority
	Q_ASSERT_COMPILE(MAX_KERNEL_AWARE_CMSIS_PRI <= (0xFF >> (8 - __NVIC_PRIO_BITS)));
} // namespace stmbot
