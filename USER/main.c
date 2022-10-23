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
 ALIENTEK ̽����STM32F407������ FreeRTOSʵ��20-1
 FreeRTOS�ڴ����ʵ��-�⺯���汾
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		256
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

#define LED0_TASK_PRIO				2
#define LED0_STK_SIZE				128
TaskHandle_t LED0Task_Handler;
void led0_task(void *p_arg);

//KEY����
#define KEY_TASK_PRIO 				3
//�����ջ��С
#define KEY_STK_SIZE				128	
//������
TaskHandle_t KEYTask_Handler;
//������
void key_task(void *p_arg);   


#define DISPLAY_TASK_PRIO				4
#define DISPLAY_STK_SIZE				128
TaskHandle_t DISPLAYTask_Handler;
void display_task(void *p_arg);

//��LCD����ʾ��ַ��Ϣ
//mode:1 ��ʾDHCP��ȡ���ĵ�ַ
//	  ���� ��ʾ��̬��ַ
void show_address(u8 mode)
{
	u8 buf[30];
	if(mode==2)
	{
		sprintf((char*)buf,"DHCP IP :%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//��ӡ��̬IP��ַ
		LCD_ShowString(30,130,210,16,16,buf); 
		sprintf((char*)buf,"DHCP GW :%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//��ӡ���ص�ַ
		LCD_ShowString(30,150,210,16,16,buf); 
		sprintf((char*)buf,"NET MASK:%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//��ӡ���������ַ
		LCD_ShowString(30,170,210,16,16,buf); 
		LCD_ShowString(30,190,210,16,16,"Port:8088!"); 
	}
	else 
	{
		sprintf((char*)buf,"Static IP:%d.%d.%d.%d",lwipdev.ip[0],lwipdev.ip[1],lwipdev.ip[2],lwipdev.ip[3]);						//��ӡ��̬IP��ַ
		LCD_ShowString(30,130,210,16,16,buf); 
		sprintf((char*)buf,"Static GW:%d.%d.%d.%d",lwipdev.gateway[0],lwipdev.gateway[1],lwipdev.gateway[2],lwipdev.gateway[3]);	//��ӡ���ص�ַ
		LCD_ShowString(30,150,210,16,16,buf); 
		sprintf((char*)buf,"NET MASK :%d.%d.%d.%d",lwipdev.netmask[0],lwipdev.netmask[1],lwipdev.netmask[2],lwipdev.netmask[3]);	//��ӡ���������ַ
		LCD_ShowString(30,170,210,16,16,buf); 
		LCD_ShowString(30,190,210,16,16,"Port:8088!"); 
	}	
}

int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	delay_init(168);					//��ʼ����ʱ����
	uart_init(115200);     				//��ʼ������
	LED_Init();		        			//��ʼ��LED�˿�
	KEY_Init();							//��ʼ������
	LCD_Init();							//��ʼ��LCD
	
	FSMC_SRAM_Init();		//SRAM��ʼ��
	
	my_mem_init(SRAMIN);            	//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		    //��ʼ���ⲿ�ڴ��
	my_mem_init(SRAMCCM);		    //��ʼ��CCM�ڴ��
	
	POINT_COLOR = RED;
	LCD_ShowString(30,30,200,20,16,"Explorer STM32F4");
	LCD_ShowString(30,50,200,20,16,"LWIP+FreeRTOS Test");
	LCD_ShowString(30,70,200,20,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,90,200,20,16,"2014/9/1");
	
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}

//��ʼ����������
void start_task(void *pvParameters)
{
	while(lwip_comm_init()) 	    //lwip��ʼ��
	{
		LCD_ShowString(30,110,200,20,16,"Lwip Init failed!"); 	//lwip��ʼ��ʧ��
		delay_ms(500);
		LCD_Fill(30,110,230,150,WHITE);
		delay_ms(500);
	}
	LCD_ShowString(30,110,200,20,16,"Lwip Init Success!"); 		//lwip��ʼ���ɹ�
		
	while(tcp_server_init()==pdFAIL)		//�������DHCP����֮���ٿ���UDP���񡣳�ʼ��udp_demo(����udp_demo�߳�)
	{
		LCD_ShowString(30,210,200,20,16,"TCPserver failed!!");	//udp����ʧ��
		delay_ms(500);
		LCD_Fill(30,210,230,170,WHITE);
		delay_ms(500);
	}		
	LCD_ShowString(30,210,200,20,16,"TCPserver Success!");		//udp�����ɹ�
	
    taskENTER_CRITICAL();           //�����ٽ���
		
	#if LWIP_DHCP
	lwip_comm_dhcp_creat(); //����DHCP�����������а�����UDP����Ŀ�����
	#endif
	
    xTaskCreate((TaskFunction_t		)led0_task,
				(const char*		)"led0_task",
				(uint16_t			)LED0_STK_SIZE,
				(void*				)NULL,
				(UBaseType_t		)LED0_TASK_PRIO,
				(TaskHandle_t*		)&LED0Task_Handler);
	//����LED0����
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
	vTaskDelete(StartTask_Handler);		//ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
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

//key����
void key_task(void *p_arg)
{
	u8 key; 
	while(1)
	{
		key = KEY_Scan(0);
		if(key==KEY0_PRES) //��������
		{
			tcp_server_flag |= LWIP_SEND_DATA; //���LWIP������Ҫ����
		}
		vTaskDelay(10);  //��ʱ10ms
	}
}
//��ʾ��ַ����Ϣ
void display_task(void *p_arg)
{
	while(1)
	{ 
#if LWIP_DHCP									//������DHCP��ʱ��
		if(lwipdev.dhcpstatus != 0) 			//����DHCP
		{
			show_address(lwipdev.dhcpstatus );	//��ʾ��ַ��Ϣ
			vTaskSuspend(DISPLAYTask_Handler); 		//��ʾ���ַ��Ϣ�������������
		}
#else
		show_address(0); 						//��ʾ��̬��ַ
		vTaskSuspend(DISPLAYTask_Handler); 			//��ʾ���ַ��Ϣ�������������
#endif //LWIP_DHCP
		vTaskDelay(500);
	}
}


