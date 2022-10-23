#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "lcd.h"
#include "sram.h"
#include "key.h"
#include "beep.h"
#include "string.h"
#include "malloc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lan8720.h"
#include "lwip/netif.h"
#include "lwip_comm.h"
#include "lwipopts.h"
#include "tcp_server_demo.h"

/************************************************
 ALIENTEK 探索者STM32F407开发板 FreeRTOS实验20-1
 FreeRTOS内存管理实验-库函数版本
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		256
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

#define LED0_TASK_PRIO				2
#define LED0_STK_SIZE				128
TaskHandle_t LED0Task_Handler;
void led0_task(void *p_arg);

//KEY任务
#define KEY_TASK_PRIO 				3
//任务堆栈大小
#define KEY_STK_SIZE				128	
//任务句柄
TaskHandle_t KEYTask_Handler;
//任务函数
void key_task(void *p_arg);   


#define DISPLAY_TASK_PRIO				4
#define DISPLAY_STK_SIZE				128
TaskHandle_t DISPLAYTask_Handler;
void display_task(void *p_arg);

//在LCD上显示地址信息
//mode:1 显示DHCP获取到的地址
//	  其他 显示静态地址
void show_address(u8 mode)
{
	u8 buf[30];
	if(mode==2)
	{
		sprintf((char*)buf,"DHCP IP :%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//打印动态IP地址
		LCD_ShowString(30,130,210,16,16,buf); 
		sprintf((char*)buf,"DHCP GW :%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//打印网关地址
		LCD_ShowString(30,150,210,16,16,buf); 
		sprintf((char*)buf,"NET MASK:%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//打印子网掩码地址
		LCD_ShowString(30,170,210,16,16,buf); 
		LCD_ShowString(30,190,210,16,16,"Port:8088!"); 
	}
	else 
	{
		sprintf((char*)buf,"Static IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//打印动态IP地址
		LCD_ShowString(30,130,210,16,16,buf); 
		sprintf((char*)buf,"Static GW:%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//打印网关地址
		LCD_ShowString(30,150,210,16,16,buf); 
		sprintf((char*)buf,"NET MASK :%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//打印子网掩码地址
		LCD_ShowString(30,170,210,16,16,buf); 
		LCD_ShowString(30,190,210,16,16,"Port:8088!"); 
	}	
}

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	delay_init(168);					//初始化延时函数
	uart_init(115200);     				//初始化串口
	LED_Init();		        			//初始化LED端口
	KEY_Init();							//初始化按键
	LCD_Init();							//初始化LCD
	
	FSMC_SRAM_Init();		//SRAM初始化
	
	my_mem_init(SRAMIN);            	//初始化内部内存池
	my_mem_init(SRAMEX);		    //初始化外部内存池
	my_mem_init(SRAMCCM);		    //初始化CCM内存池
	
	POINT_COLOR = RED;
	LCD_ShowString(30,30,200,20,16,"Explorer STM32F4");
	LCD_ShowString(30,50,200,20,16,"LWIP+FreeRTOS Test");
	LCD_ShowString(30,70,200,20,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,90,200,20,16,"2014/9/1");
	
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{
	while(lwip_comm_init()) 	    //lwip初始化
	{
		LCD_ShowString(30,110,200,20,16,"Lwip Init failed!"); 	//lwip初始化失败
		delay_ms(500);
		LCD_Fill(30,110,230,150,WHITE);
		delay_ms(500);
	}
	LCD_ShowString(30,110,200,20,16,"Lwip Init Success!"); 		//lwip初始化成功
		
	while(tcp_server_init()==pdFAIL)		//当完成了DHCP任务之后，再开启UDP任务。初始化udp_demo(创建udp_demo线程)
	{
		LCD_ShowString(30,210,200,20,16,"TCPserver failed!!");	//udp创建失败
		delay_ms(500);
		LCD_Fill(30,210,230,170,WHITE);
		delay_ms(500);
	}		
	LCD_ShowString(30,210,200,20,16,"TCPserver Success!");		//udp创建成功
	
    taskENTER_CRITICAL();           //进入临界区
		
	#if LWIP_DHCP
	lwip_comm_dhcp_creat(); //创建DHCP任务，这里面有包含了UDP任务的开启。
	#endif
	
    xTaskCreate((TaskFunction_t		)led0_task,
				(const char*		)"led0_task",
				(uint16_t			)LED0_STK_SIZE,
				(void*				)NULL,
				(UBaseType_t		)LED0_TASK_PRIO,
				(TaskHandle_t*		)&LED0Task_Handler);
	//创建LED0任务
	xTaskCreate((TaskFunction_t		)key_task,
				(const char*		)"key_task",
				(uint16_t			)KEY_STK_SIZE,
				(void*				)NULL,
				(UBaseType_t		)KEY_TASK_PRIO,
				(TaskHandle_t*		)&KEYTask_Handler);
	xTaskCreate((TaskFunction_t		)display_task,
				(const char*		)"display_task",
				(uint16_t			)DISPLAY_STK_SIZE,
				(void*				)NULL,
				(UBaseType_t		)DISPLAY_TASK_PRIO,
				(TaskHandle_t*		)&DISPLAYTask_Handler);
	vTaskDelete(StartTask_Handler);		//删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

void led0_task(void *p_arg)
{
	while(1)
	{
		LED0=~LED0;
		vTaskDelay(500);
		//delay_xms(500);
	}
}

//key任务
void key_task(void *p_arg)
{
	u8 key; 
	while(1)
	{
		key = KEY_Scan(0);
		if(key==KEY0_PRES) //发送数据
		{
			tcp_server_flag |= LWIP_SEND_DATA; //标记LWIP有数据要发送
		}
		vTaskDelay(10);  //延时10ms
	}
}
//显示地址等信息
void display_task(void *p_arg)
{
	while(1)
	{ 
#if LWIP_DHCP									//当开启DHCP的时候
		if(lwipdev.dhcpstatus != 0) 			//开启DHCP
		{
			show_address(lwipdev.dhcpstatus );	//显示地址信息
			vTaskSuspend(DISPLAYTask_Handler); 		//显示完地址信息后挂起自身任务
		}
#else
		show_address(0); 						//显示静态地址
		vTaskSuspend(DISPLAYTask_Handler); 			//显示完地址信息后挂起自身任务
#endif //LWIP_DHCP
		vTaskDelay(500);
	}
}


