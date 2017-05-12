/**
  ******************************************************************************


  ******************************************************************************
  */ 
	
//#include "stm32f10x.h"
//#include "bsp_led.h"
//#include "usart1.h"
//#include "bsp.h"
//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
#include "includes.h"


static void vTaskTaskUserIF(void *pvParameters);
static void vTaskLED(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate(void);

static TaskHandle_t xHandleTaskUserIF = NULL;
static TaskHandle_t xHandleTaskLED = NULL;
static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskStart = NULL;



int main(void)
{	


	__set_PRIMASK(1);
	bsp_Init(); //硬件初始化

	vSetupSysInfoTest();//为了检测系统任务信息，时钟精度要高于系统节拍
	AppTaskCreate();
	vTaskStartScheduler();
	while(1);

}


static void vTaskTaskUserIF(void *pvParameters)
{
	while(1)
	{
		macLED1_ON ();	
		vTaskDelay(4000);
		macLED1_OFF ();	
		vTaskDelay(4000);
	}
}
static void vTaskLED(void *pvParameters)
{
	while(1)
	{
		macLED2_TOGGLE(); 
		vTaskDelay(400);
	}
}
static void vTaskMsgPro(void *pvParameters)
{
	uint8_t ucKeyCode;
	uint8_t pcWriteBuffer[500];

    while(1)
    {
		ucKeyCode = Key_Scan(GPIOA,GPIO_Pin_0) ;
		
		if (ucKeyCode != KEY_OFF)
		{
			switch (ucKeyCode)
			{
				/* K1键按下 打印任务执行情况 */
				case KEY_ON:			 
					printf("=================================================\r\n");
					printf("Taskname    state priority  stacksize TaskNum\r\n"); 
					vTaskList((char *)&pcWriteBuffer);
					printf("%s\r\n", pcWriteBuffer);
				
					printf("\r\nTask name       Run count       Use rate\r\n");
					vTaskGetRunTimeStats((char *)&pcWriteBuffer);
					printf("%s\r\n", pcWriteBuffer);
					break;
				
				/* 其他的键值不处理 */
				default:                     
					break;
			}
		}
		
		vTaskDelay(20);
	}
}
 static void vTaskStart(void *pvParameters)
{
	while(1)
	{

	
		USART1_printf(USART1, "\r\n ("__DATE__ " - " __TIME__ ") \r\n");
		vTaskDelay(2000);
	}
} 
static void AppTaskCreate(void)
{
	xTaskCreate(vTaskTaskUserIF,
				"vTaskTaskUserIF",
				512,
				NULL,
				1,
				&xHandleTaskUserIF
				);
	xTaskCreate(vTaskLED,
				"vTaskLED",
				512,
				NULL,
				2,
				&xHandleTaskLED
				);
	xTaskCreate(vTaskMsgPro,
				"vTaskMsgPro",
				512,
				NULL,
				3,
				&xHandleTaskMsgPro
				);
	xTaskCreate(vTaskStart,
				"vTaskStart",
				512,
				NULL,
				4,
				&xHandleTaskStart
				); 
}



/*********************************************END OF FILE**********************/












