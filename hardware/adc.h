#ifndef _ADC_H
#define _ADC_H

#include "stm32f10x.h"

//ADC1_BASE地址：0x4000 0000+0x2400 即ADC1的基地址，而他的规则数据寄存器的偏移地址是：0x4c
#define ADC1_DR_Address    ((u32)ADC1_BASE+0x4c)


void ADCx_Init(void);













































#endif





