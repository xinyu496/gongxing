#include "stm32f10x.h"
#include "cpu.h"
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>   // 用于获取时间种子

DateTime p_clockDisplay;

 /* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* LED1任务句柄 */
static TaskHandle_t LED1_Task_Handle = NULL;
/* LED2任务句柄 */
static TaskHandle_t LED2_Task_Handle = NULL;
/* beep任务句柄 */
static TaskHandle_t beepTask_Handle = NULL;

static TaskHandle_t CPU_Task_Handle = NULL;

static TaskHandle_t dacTask_Handle = NULL;

extern __IO uint16_t ADC_ConvertedValue[2];

/**********************************************************************
  * @ 函数名  ： LED_Task
  * @ 功能说明： LED_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void LED1_Task(void* parameter)
{	
    while (1)
    {
			
				GPIO_ResetBits(GPIOB , GPIO_Pin_5);  
					
        vTaskDelay(500);   /* 延时500个tick */		 		
        printf("LED1_Task Running,LED1_OFF\r\n");
			
        GPIO_SetBits(GPIOB , GPIO_Pin_5);
		
        vTaskDelay(500);   /* 延时500个tick */
        printf("LED1_Task Running,LED1_ON\r\n");
        
        
			
			
    }
}

/**********************************************************************
  * @ 函数名  ： LED_Task
  * @ 功能说明： LED_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void LED2_Task(void* parameter)
{	
    while (1)
    {
			
			GPIO_ResetBits(GPIOB , GPIO_Pin_0);     
        vTaskDelay(500);   /* 延时500个tick */		 		
        printf("LED2_Task Running,LED2_OFF\r\n");
			
        GPIO_SetBits(GPIOB , GPIO_Pin_0);
				RTC_GetDateTime(&p_clockDisplay);
        vTaskDelay(500);   /* 延时500个tick */
        printf("LED2_Task Running,LED2_ON\r\n");
        
        
    }
}

/**********************************************************************
  * @ 函数名  ： beepTask
  * @ 功能说明： beepTask任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
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
  uint8_t CPU_RunInfo[400];		//保存任务运行时间信息
	
  
  while (1)
  {
    memset(CPU_RunInfo,0,400);				//信息缓冲区清零
    
    vTaskList((char *)&CPU_RunInfo);  //获取任务运行时间信息
    
    printf("---------------------------------------------\r\n");
		
    printf("任务名      任务状态 优先级   剩余栈 任务序号\r\n");
    printf("%s", CPU_RunInfo);
    printf("---------------------------------------------\r\n");
    
    memset(CPU_RunInfo,0,400);				//信息缓冲区清零
    
    vTaskGetRunTimeStats((char *)&CPU_RunInfo);
    
    printf("任务名       运行计数         利用率\r\n");
    printf("%s", CPU_RunInfo);
    printf("---------------------------------------------\r\n\n");
		
		
	

    vTaskDelay(1000);   /* 延时500个tick */		
  }
}

//dac任务  秒更新
static void dacTask(void* parameter)
{	
	unsigned short data;
	
	while(1)
	{
		srand((unsigned int)xTaskGetTickCount());  // xTaskGetTickCount() 获取FreeRTOS系统时间

		int random_num = rand() % 4096;  // 生成0-99的随机数
		
		data = (unsigned short)random_num;
		
		DAC_SetChannel1Data(DAC_Align_12b_R , data);
		
		printf("dacTask Running,LED1_OFF\r\n");
		printf("dac value is%d",ADC_ConvertedValue[0]);
		vTaskDelay(1000);   /* 延时500个tick */		
	}
}
/***********************************************************************
  * @ 函数名  ： AppTaskCreate
  * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  
  taskENTER_CRITICAL();           //进入临界区
  
  /* 创建LED_Task任务 */
  xReturn = xTaskCreate((TaskFunction_t )LED1_Task, /* 任务入口函数 */
                        (const char*    )"LED1_Task",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )2,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&LED1_Task_Handle);/* 任务控制块指针 */
  if(pdPASS == xReturn)
    printf("创建LED1_Task任务成功!\r\n");
  
	/* 创建LED_Task任务 */
  xReturn = xTaskCreate((TaskFunction_t )LED2_Task, /* 任务入口函数 */
                        (const char*    )"LED2_Task",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )3,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&LED2_Task_Handle);/* 任务控制块指针 */
  if(pdPASS == xReturn)
    printf("创建LED2_Task任务成功!\r\n");
	
	/* 创建LED_Task任务 */
  xReturn = xTaskCreate((TaskFunction_t )beepTask, /* 任务入口函数 */
                        (const char*    )"beepTask",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )3,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&beepTask_Handle);/* 任务控制块指针 */
  if(pdPASS == xReturn)
    printf("创建beepTask任务成功!\r\n");
	
	/* 创建CPU_Task任务 */
  xReturn = xTaskCreate((TaskFunction_t )CPU_Task, /* 任务入口函数 */
                        (const char*    )"CPU_Task",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )4,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&CPU_Task_Handle);/* 任务控制块指针 */
  if(pdPASS == xReturn)
    printf("创建CPU_Task任务成功!\r\n");
	
	/* 创建dacTask任务 */
  xReturn = xTaskCreate((TaskFunction_t )dacTask, /* 任务入口函数 */
                        (const char*    )"dacTask",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )4,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&dacTask_Handle);/* 任务控制块指针 */
  if(pdPASS == xReturn)
    printf("创建dacTask任务成功!\r\n");
  
  vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务
  
  taskEXIT_CRITICAL();            //退出临界区
}





int main(void)
{
	double xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	
	initCpu();
	
	 /* 创建AppTaskCreate任务 */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* 任务入口函数 */
                        (const char*    )"AppTaskCreate",/* 任务名字 */
                        (uint16_t       )512,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )1, /* 任务的优先级 */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* 任务控制块指针 */ 
												
	  /* 启动任务调度 */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* 启动任务，开启调度 */
  else
    return -1; 											
	

	while (1) {
			
	}
}



