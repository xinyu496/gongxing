#include "stm32f10x.h"
#include "cpu.h"

DateTime p_clock;

int main(void)
{
//	initCpu();
//	while(1)
//	{
//		
//	}


	initCpu();
	setOriginDate();//设置初始时间；仅第一次使用
	

	
	while (1) {
			RTC_GetDateTime(&p_clock);
	}
}



