#include "stm32f10x.h"
#include "cpu.h"
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>   // ���ڻ�ȡʱ������

DateTime p_clockDisplay;

 /* ���������� */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED1������ */
static TaskHandle_t LED1_Task_Handle = NULL;
/* LED2������ */
static TaskHandle_t LED2_Task_Handle = NULL;
/* beep������ */
static TaskHandle_t beepTask_Handle = NULL;

static TaskHandle_t CPU_Task_Handle = NULL;

static TaskHandle_t dacTask_Handle = NULL;

extern __IO uint16_t ADC_ConvertedValue[2];

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
				vTaskDelay(3000);
			
				printf("beepTask Running,LED1_OFF\r\n");
    }
}

static void CPU_Task(void* parameter)
{	
  uint8_t CPU_RunInfo[400];		//������������ʱ����Ϣ
	
  
  while (1)
  {
    memset(CPU_RunInfo,0,400);				//��Ϣ����������
    
    vTaskList((char *)&CPU_RunInfo);  //��ȡ��������ʱ����Ϣ
    
    printf("---------------------------------------------\r\n");
		
    printf("������      ����״̬ ���ȼ�   ʣ��ջ �������\r\n");
    printf("%s", CPU_RunInfo);
    printf("---------------------------------------------\r\n");
    
    memset(CPU_RunInfo,0,400);				//��Ϣ����������
    
    vTaskGetRunTimeStats((char *)&CPU_RunInfo);
    
    printf("������       ���м���         ������\r\n");
    printf("%s", CPU_RunInfo);
    printf("---------------------------------------------\r\n\n");
		
		
	

    vTaskDelay(1000);   /* ��ʱ500��tick */		
  }
}

//dac����  �����
static void dacTask(void* parameter)
{	
	unsigned short data;
	
	while(1)
	{
		srand((unsigned int)xTaskGetTickCount());  // xTaskGetTickCount() ��ȡFreeRTOSϵͳʱ��

		int random_num = rand() % 4096;  // ����0-99�������
		
		data = (unsigned short)random_num;
		
		DAC_SetChannel1Data(DAC_Align_12b_R , data);
		
		printf("dacTask Running,LED1_OFF\r\n");
		printf("dac value is%d",ADC_ConvertedValue[0]);
		vTaskDelay(1000);   /* ��ʱ500��tick */		
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
    printf("����beepTask����ɹ�!\r\n");
	
	/* ����CPU_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )CPU_Task, /* ������ں��� */
                        (const char*    )"CPU_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )4,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&CPU_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����CPU_Task����ɹ�!\r\n");
	
	/* ����dacTask���� */
  xReturn = xTaskCreate((TaskFunction_t )dacTask, /* ������ں��� */
                        (const char*    )"dacTask",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )4,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&dacTask_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����dacTask����ɹ�!\r\n");
  
  vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
  
  taskEXIT_CRITICAL();            //�˳��ٽ���
}





int main(void)
{
	double xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	
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
			
	}
}



