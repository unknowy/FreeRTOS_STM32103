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
static void Overflowtest(void);

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

		printf("=================================================\r\n");

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
		macLED1_ON ();	
		vTaskDelay(400);
		macLED1_OFF ();
		vTaskDelay(400);
	}
}
 static void vTaskStart(void *pvParameters)
{
	uint8_t ucKeyCodek1;
	uint8_t ucKeyCodek2;
	uint8_t ucKeyCode;
	uint8_t pcWriteBuffer[500];

    while(1)
    {
		ucKeyCodek1 = Key_Scan(GPIOC,GPIO_Pin_13);
		ucKeyCodek2 = Key_Scan(GPIOA,GPIO_Pin_0);
		if (ucKeyCodek1==KEY_ON&&ucKeyCodek2==KEY_ON)
			ucKeyCode=0x01;
		if (ucKeyCodek1==KEY_OFF&&ucKeyCodek2==KEY_OFF)
			ucKeyCode=0x10;	
		if (ucKeyCodek1==KEY_OFF&&ucKeyCodek2==KEY_ON)
			ucKeyCode=0x11;	
		if (ucKeyCodek1==KEY_ON&&ucKeyCodek2==KEY_OFF)
			ucKeyCode=0x00;			
		if (ucKeyCode != 0x00)
		{
			switch (ucKeyCode)
			{
				/* K1键按下 打印任务执行情况 */
				case 0x01:			 
							printf("=================================================\r\n");
							printf("Taskname    state priority  stacksize TaskNum\r\n"); 
							vTaskList((char *)&pcWriteBuffer);
							printf("%s\r\n", pcWriteBuffer);
						
							printf("\r\nTask name       Run count       Use rate\r\n");
							vTaskGetRunTimeStats((char *)&pcWriteBuffer);
							printf("%s\r\n", pcWriteBuffer);
						
							USART1_printf(USART1, "\r\n ("__DATE__ " - " __TIME__ ") \r\n");
					break;

				case 0x10:			 
					printf("gg overflow\r\n");
					Overflowtest();
					break;
				/* 其他的键值不处理 */
				default:   
					printf("%x\r\n",ucKeyCode);                  
					break;
			}
		}
		
		vTaskDelay(200) ;
	}
} 
static void Overflowtest(void)
{
	int16_t i;
	uint8_t buf[2048];
	for(i=2047;i>=0;i--)
	{
		buf[i]=0x55;
		vTaskDelay(1);
	}
}
void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName )
{
	printf("%s overflow \r\n", pcTaskName);
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












