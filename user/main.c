#include "stm32f10x.h"
#include "cpu.h"
#include "FreeRTOS.h"
#include "task.h"

DateTime p_clockDisplay;

 /* ���������� */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED1������ */
static TaskHandle_t LED1_Task_Handle = NULL;
/* LED2������ */
static TaskHandle_t LED2_Task_Handle = NULL;
/* beep������ */
static TaskHandle_t beepTask_Handle = NULL;

/**********************************************************************
  * @ ������  �� LED_Task
  * @ ����˵���� LED_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void LED1_Task(void* parameter)
{	
    while (1)
    {
			
				GPIO_ResetBits(GPIOB , GPIO_Pin_5);  
					
        vTaskDelay(500);   /* ��ʱ500��tick */		 		
        printf("LED1_Task Running,LED1_OFF\r\n");
			
        GPIO_SetBits(GPIOB , GPIO_Pin_5);
		
        vTaskDelay(500);   /* ��ʱ500��tick */
        printf("LED1_Task Running,LED1_ON\r\n");
        
        
			
			
    }
}

/**********************************************************************
  * @ ������  �� LED_Task
  * @ ����˵���� LED_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void LED2_Task(void* parameter)
{	
    while (1)
    {
			
			GPIO_ResetBits(GPIOB , GPIO_Pin_0);     
        vTaskDelay(500);   /* ��ʱ500��tick */		 		
        printf("LED2_Task Running,LED2_OFF\r\n");
			
        GPIO_SetBits(GPIOB , GPIO_Pin_0);
				RTC_GetDateTime(&p_clockDisplay);
        vTaskDelay(500);   /* ��ʱ500��tick */
        printf("LED2_Task Running,LED2_ON\r\n");
        
        
    }
}

/**********************************************************************
  * @ ������  �� beepTask
  * @ ����˵���� beepTask��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void beepTask(void* parameter)
{	
    while (1)
    {
				beepOn_Off(1);		
				vTaskDelay(100);
				beepOn_Off(0);		
				vTaskDelay(100);
				beepOn_Off(1);		
				vTaskDelay(100);
				beepOn_Off(0);		
				vTaskDelay(100);
				beepOn_Off(1);		
				vTaskDelay(100);
				beepOn_Off(0);		
				vTaskDelay(1000);
			
        
        
    }
}
/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  taskENTER_CRITICAL();           //�����ٽ���
  
  /* ����LED_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )LED1_Task, /* ������ں��� */
                        (const char*    )"LED1_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )2,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&LED1_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����LED1_Task����ɹ�!\r\n");
  
	/* ����LED_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )LED2_Task, /* ������ں��� */
                        (const char*    )"LED2_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )3,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&LED2_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����LED2_Task����ɹ�!\r\n");
	
	/* ����LED_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )beepTask, /* ������ں��� */
                        (const char*    )"beepTask",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )3,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&beepTask_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����LED2_Task����ɹ�!\r\n");
  
  vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
  
  taskEXIT_CRITICAL();            //�˳��ٽ���
}





int main(void)
{
	double xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	char AD;
	initCpu();
	
	 /* ����AppTaskCreate���� */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                        (const char*    )"AppTaskCreate",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )1, /* ��������ȼ� */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 
												
	  /* ����������� */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* �������񣬿������� */
  else
    return -1; 											
	

	while (1) {
			AD = 1;
	}
}



