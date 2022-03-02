#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usart3.h"
#include "esp8266.h"
#include "string.h"
#include "timer.h"
#include "timer_mos.h"
#include "IOI2C.h"
#include "MPU6050.h"
 
/*
项目的主要内容：STM32配合ESP8266模块与服务器数据交互

ESP8266的连接：USART3（PB10、PB11）

如何判断数据接收完全？
1、出现了换行符；
2、如果超过10ms了都没有下一条数据（TIM7来进行10ms的定时）。
*/

int strict(int a)
{
	if(a>710)
		a=710;
	else if(a<10)
		a=10;
	return a;
}
 int main(void)
 {

	 char* ack;
	 int led0pwmval=719;//0-719
	 int pwmi=0;
	 int pwmk=0;
	 int P_I=0;
	 int P_K=0;
	 int D_I=0;
	 int D_K=0;
	 u8 state=1;
	 int anglei;
	 int anglek;
	 int anglej;
	 int anglei_last=0;
	 int anglek_last=0;
	 float *p;
	 char buf[100];	
	 
	delay_init();	    	 			//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 			//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 				//串口初始化为115200
	usart3_init(115200);	 				//串口初始化为115200

	esp8266_start_trans();							//esp8266进行初始化
	 
	//esp8266_send_data("ready",50);
	 
	//esp8266_quit_trans();
	 //esp8266_send_data("去吃饭啊",50);
	 IIC_Init();                     //IIC初始化
	MPU6050_initialize();           //MPU6050初始化	
	DMP_Init(); 
	 
	 
  TIM3_PWM_Init(719,99);	 //不分频。PWM频率=72000000/72000=1Khz
	TIM_SetCompare1(TIM3,strict(719));
TIM_SetCompare2(TIM3,strict(719));
TIM_SetCompare3(TIM3,strict(719));
TIM_SetCompare4(TIM3,strict(719));
	delay_ms(1000);
	USART3_RX_STA=0;
 	while(1)
	{
		if(state==1)
		{
			p = Read_DMP();		 
		  anglei=p[0];	
		  anglek=p[1];
		//memset(buf,0,100);
		//sprintf(buf,"angle1=%d,angle2=%d,angle3=%d",anglei,anglek,anglej);
		//printf("%s\r\n",buf);
		pwmi=(int)((anglei*P_I+(anglei-anglei_last)*D_I)/100);
		pwmk=(int)((anglek*P_K+(anglek-anglek_last)*D_K)/100);
		anglei_last=anglei;
		anglek_last=anglek;
			
			//printf("%d\r\n",strict(led0pwmval-pwmi-pwmk));
		
		TIM_SetCompare1(TIM3,strict(led0pwmval-pwmi-pwmk));
TIM_SetCompare2(TIM3,strict(led0pwmval-pwmi+pwmk));
TIM_SetCompare3(TIM3,strict(led0pwmval+pwmi+pwmk));
TIM_SetCompare4(TIM3,strict(led0pwmval+pwmi-pwmk));
		//printf("%d\r\n",(int)led0pwmval);
		if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
				ack=(char*)USART3_RX_BUF;
				printf("ack:%s\r\n",(u8*)ack);
				//printf("ack:%s\r\n",ack);
				printf("ack1:%d\r\n",*((u8*)ack+6)-48);
				printf("ack2:%d\r\n",*((u8*)ack+7)-48);
				printf("ack3:%d\r\n",*((u8*)ack+8)-48);
				led0pwmval = 719 - ((*(ack+6)-48)*100+(*(ack+7)-48)*10+(*(ack+8)-48)*1);
				printf("%d\r\n",led0pwmval);
				D_I = (*(ack+3)-48)*100+(*(ack+4)-48)*10+(*(ack+5)-48)*1;
				P_I = (*(ack+0)-48)*100+(*(ack+1)-48)*10+(*(ack+2)-48)*1;
				P_K=P_I;
				D_K=D_I;
				USART3_RX_STA=0;
			}
		}
		else if(state==0)
		{
			TIM_SetCompare1(TIM3,719);
TIM_SetCompare2(TIM3,719);
TIM_SetCompare3(TIM3,719);
TIM_SetCompare4(TIM3,719);
		}
		//esp8266_send_data("去吃饭啊",50);
		//GPIO_SetBits(GPIOA,GPIO_Pin_1);
		delay_ms(5);
	}
 }

 
 

