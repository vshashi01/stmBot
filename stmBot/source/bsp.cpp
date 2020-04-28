#include "bsp.h"
#include "isr_priority.h"
#include "stmClock/stmClock.h"
#include "stmCortexFunction/stmCortexFunction.h"
#include "stmEXTI/stm_exti.h"
#include "stmFlash/stmFlash.h"

namespace
{
	SysClock systemClock(SYSCLK_SOURCE::SYSCLK_SOURCE_PLL,
						 AHB_DIVIDER::AHB_SYSCLK_Div_NO,
						 APB1_DIVIDER::APB1_HCLK_Div_2,
						 APB2_DIVIDER::APB2_HCLK_Div_NO,
						 FlashLatencyWait::FLASH_LATENCY_WAIT_2);
}

void BSP::initBoard()
{
	// enable prefetch buffer
	enablePrefetchBuffer();
	// if not enabled exit , should be set to NVICSystemReset()
	if (!isEnabledPrefetchBuffer())
		return;

	SystemCoreClockUpdate();
}

void BSP::setupClock()
{
	stmcortexfunction::setNVICPriorityGroup(NVIC_PRIORITY_GROUP_4);

	InitTick(SystemCoreClock, 1000);

	systemClock.setPLL(PLL_SOURCE_HSE, PLL_HSE_DIV_NO, PLLMUL_9);
	systemClock.setup(1000, stmbot::SYSTICK_PRIO);

	delay(10);
}

void BSP::enableSysTickHandler()
{
	systemClock.enableSysTick(stmbot::SYSTICK_PRIO, 1000);
}