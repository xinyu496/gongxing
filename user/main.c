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
	setOriginDate();//���ó�ʼʱ�䣻����һ��ʹ��
	

	
	while (1) {
			RTC_GetDateTime(&p_clock);
	}
}



