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
	setOriginDate();//���ó�ʼʱ�䣻����һ��ʹ��
	

	
	while (1) {
			RTC_GetDateTime(&dt);
	}
}



