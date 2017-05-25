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
static void AppObjCreate(void);
static void vTimerCallback(xTimerHandle pxTimer);

static TaskHandle_t xHandleTaskUserIF = NULL;
static TaskHandle_t xHandleTaskLED = NULL;
static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskStart = NULL;
static TimerHandle_t xTimers[2] = {NULL};

int main(void)
{

	__set_PRIMASK(1);
	bsp_Init(); //硬件初始化

	vSetupSysInfoTest(); //为了检测系统任务信息，时钟精度要高于系统节拍
	AppTaskCreate();
	AppObjCreate(); /* 创建任务通信机制 */
	vTaskStartScheduler();
	while (1)
		;
}

static void vTaskTaskUserIF(void *pvParameters)
{

	while (1)
	{

		printf("=================================================\r\n");

		vTaskDelay(4000);
	}
}

static void vTaskLED(void *pvParameters)
{
	while (1)
	{

		printf("=================================================\r\n");

		vTaskDelay(4000);
	}
	//	TickType_t xLastWakeTime;
	//	const TickType_t xFrequency = 200;

	//	/* 获取当前的系统时间 */
	//    xLastWakeTime = xTaskGetTickCount();
	//
	//    while(1)
	//    {
	//		macLED1_OFF();
	//
	//		/* vTaskDelayUntil是绝对延迟，vTaskDelay是相对延迟。*/
	//        vTaskDelayUntil(&xLastWakeTime, xFrequency);
}

static void vTaskMsgPro(void *pvParameters)
{
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 200;

	/* 获取当前的系统时间 */
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		macLED2_TOGGLE();
		macLED3_TOGGLE();
		/* vTaskDelayUntil是绝对延迟，vTaskDelay是相对延迟。*/
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
static void vTaskStart(void *pvParameters)
{
	uint8_t ucKeyCodek1;
	uint8_t ucKeyCodek2;
	uint8_t ucKeyCode;
	uint8_t pcWriteBuffer[500];

	while (1)
	{
		ucKeyCodek1 = Key_Scan(GPIOC, GPIO_Pin_13);
		ucKeyCodek2 = Key_Scan(GPIOA, GPIO_Pin_0);
		if (ucKeyCodek1 == KEY_ON && ucKeyCodek2 == KEY_ON)
			ucKeyCode = 0x01;
		if (ucKeyCodek1 == KEY_OFF && ucKeyCodek2 == KEY_OFF)
			ucKeyCode = 0x10;
		if (ucKeyCodek1 == KEY_OFF && ucKeyCodek2 == KEY_ON)
			ucKeyCode = 0x11;
		if (ucKeyCodek1 == KEY_ON && ucKeyCodek2 == KEY_OFF)
			ucKeyCode = 0x00;
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

				USART1_printf(USART1, "\r\n ("__DATE__
									  " - " __TIME__ ") \r\n");
				break;

			default:
				printf("%x\r\n", ucKeyCode);
				break;
			}
		}

		vTaskDelay(20);
	}
}

static void AppTaskCreate(void)
{
	xTaskCreate(vTaskTaskUserIF,
				"vTaskTaskUserIF",
				512,
				NULL,
				1,
				&xHandleTaskUserIF);
	xTaskCreate(vTaskLED,
				"vTaskLED",
				512,
				NULL,
				2,
				&xHandleTaskLED);
	xTaskCreate(vTaskMsgPro,
				"vTaskMsgPro",
				512,
				NULL,
				3,
				&xHandleTaskMsgPro);
	xTaskCreate(vTaskStart,
				"vTaskStart",
				512,
				NULL,
				4,
				&xHandleTaskStart);
}
/*
*********************************************************************************************************
*	函 数 名: AppObjCreate
*	功能说明: 创建任务通信机制
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppObjCreate(void)
{
	uint8_t i;
	const TickType_t xTimerPer[2] = {20000, 10000};

	/* 
	  1. 创建定时器，如果在RTOS调度开始前初始化定时器，那么系统启动后才会执行。
	  2. 统一初始化两个定时器，他们使用共同的回调函数，在回调函数中通过定时器ID来区分
	     是那个定时器的时间到。当然，使用不同的回调函数也是没问题的。
	*/
	for (i = 0; i < 2; i++)
	{
		xTimers[i] = xTimerCreate("Timer",		   /* 定时器名字 */
								  xTimerPer[i],	/* 定时器周期,单位时钟节拍 */
								  pdTRUE,		   /* 周期性 */
								  (void *)i,	   /* 定时器ID */
								  vTimerCallback); /* 定时器回调函数 */

		if (xTimers[i] == NULL)
		{
			/* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
		}
		else
		{
			/* 启动定时器，系统启动后才开始工作 */
			if (xTimerStart(xTimers[i], 100) != pdPASS)
			{
				/* 定时器还没有进入激活状态 */
			}
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: vTimerCallback
*	功能说明: 定时器回调函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void vTimerCallback(xTimerHandle pxTimer)
{
	uint32_t ulTimerID;

	configASSERT(pxTimer);

	/* 获取那个定时器时间到 */
	ulTimerID = (uint32_t)pvTimerGetTimerID(pxTimer);

	/* 处理定时器0任务 */
	if (ulTimerID == 0)
	{
		macLED1_ON();
		printf("0000000000\r\n");
	}

	/* 处理定时器1任务 */
	if (ulTimerID == 1)
	{
		macLED1_ON();
		printf("1111111111\r\n");
	}
}

/*********************************************END OF FILE**********************/
