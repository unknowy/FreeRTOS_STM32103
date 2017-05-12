/**
  ******************************************************************************


  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"
#include "usart1.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"



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
	//_set_PRIMASK(1);
	
	/* LED 端口初始化 */
	LED_Init ();	          //初始化 LED
	USART1_Config();
	
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
	while(1)
	{
		macLED3_TOGGLE(); 
		vTaskDelay(400);
	}
}
 static void vTaskStart(void *pvParameters)
{
	while(1)
	{
		printf("\r\n this is a printf demo \r\n");

		printf("\r\n 欢迎使用野火M3实验板:) \r\n");
		
		USART1_printf(USART1, "\r\n This is a USART1_printf demo \r\n");
	
		USART1_printf(USART1, "\r\n ("__DATE__ " - " __TIME__ ") \r\n");
		vTaskDelay(4000);
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












