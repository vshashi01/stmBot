#ifndef _BSP_H
#define _BSP_H

#include "qpcpp.h"
#include "stm32f103xb.h"
#include "stm_app.h"

namespace BSP
{
	void initBoard();
	void setupClock();
	void enableSysTickHandler();
} // namespace BSP

#endif