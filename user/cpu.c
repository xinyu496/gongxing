#include "cpu.h"
#include "rtc.h"

//中断初始化
static void interruptInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	// 设置中断组为0
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
	// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn ;	
	// 设置主优先级为 0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	// 设置抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//时钟初始化
static void periphClockInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//蜂鸣器挂载时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);//10ms定时器挂载时钟
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//蜂鸣器Io挂载时钟
}




//控制蜂鸣器开关
void beepOn_Off(bool state)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if(state == ON)
	{
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
	else if(state == OFF)
	{
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
}




















//外设初始化
void initCpu(void)
{
	periphClockInit();//时钟初始化
	interruptInit();//中断初始化
	
	timInit();//定时器初始化
	RTC_Init();
}










