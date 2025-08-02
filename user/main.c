#include "stm32f10x.h"
#include "cpu.h"



int main(void)
{
//	initCpu();
//	while(1)
//	{
//		
//	}
	DateTime dt;

	initCpu();
	setOriginDate();//设置初始时间；仅第一次使用
	

	
	while (1) {
			RTC_GetDateTime(&dt);
	}
}



