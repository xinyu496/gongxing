#ifndef _CPU_H
#define _CPU_H

#include "stm32f10x.h"
#include "tim.h"
#include "rtc.h"
#include "dataconfig.h"
#include "gpio.h"
#include "uart.h"



void beepOn_Off(u8 state);
void initCpu(void);


#endif




