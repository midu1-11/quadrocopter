#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usart3.h"
#include "esp8266.h"
#include "string.h"
#include "timer.h"
#include "oled.h"
#include "adc.h"
 
char ack[10]="0000000000";

void create_string(int a,int k)
{
	if(k==0)
	{
		*(ack+0)=a/100+48;
		*(ack+1)=(a/10)%10+48;
		*(ack+2)=a%10+48;
	}
	else if(k==1)
	{
		*(ack+3)=a/100+48;
		*(ack+4)=(a/10)%10+48;
		*(ack+5)=a%10+48;
	}
	else if(k==2)
	{
		*(ack+6)=a/100+48;
		*(ack+7)=(a/10)%10+48;
		*(ack+8)=a%10+48;
	}
}


 int main(void)
 {
	 int p=0,d=0,pwm=400;
	 int selected = 0;
	 int change = 0;
	 int analog1,analog2,analog3,analog4;
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 			//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	 uart_init(115200);	 				//串口初始化为115200
	usart3_init(115200);	 				//串口初始化为115200
	 Adc_Init();

	esp8266_start_trans();							//esp8266进行初始化
	 
	OLED_Init();
	OLED_ColorTurn(0);//0正常显示，1 反色显示
  OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
//	OLED_DrawPoint(0,0);
//	OLED_DrawLine(20,0,50,60);
//	OLED_DrawCircle(64,32,20);
	OLED_Refresh();
	 
	 OLED_Clear();
		OLED_ShowString(0,0,"P",16);
		OLED_ShowNum(36,0,p,3,16);
	  OLED_ShowString(0,16,"I",16);
		OLED_ShowNum(36,16,d,3,16);
		OLED_ShowString(0,32,"PWM",16);
		OLED_ShowNum(36,32,pwm,3,16);
		if(selected==0)
			OLED_ShowString(76,0,"on",16);
		else if(selected==1)
			OLED_ShowString(76,16,"on",16);
		else if(selected==2)
			OLED_ShowString(76,32,"on",16);
	  OLED_Refresh();
	 
	 
	while(1)
	{
		
  	
		
		
		
		analog1 = Get_Adc_Average(1,1);
		analog2 = Get_Adc_Average(4,1);
		analog3 = Get_Adc_Average(5,1);
		analog4 = Get_Adc_Average(6,1);
		
		printf("%d\r\n",analog1);
		printf("%d\r\n",analog2);
		printf("%d\r\n",analog3);
		printf("%d\r\n\r\n",analog4);
		
		if(analog4<1000)
		{
			selected--;
			change=1;
			if(selected==-1)
				selected=2;
		}
		else if(analog4>3000)
		{
			selected++;
			change=1;
			if(selected==3)
				selected=0;
		}
		
		if(analog2>2400)
		{
			if(selected==0)
			{
				p+=10/(1700/(analog2-2400));
				change=1;
				if(p>300)
					p=300;
			}
			else if(selected==1)
			{
				d+=10/(1700/(analog2-2400));
				change=1;
				if(d>300)
					d=300;
			}
			if(selected==2)
			{
				pwm+=10/(1700/(analog2-2400));
				change=1;
				if(pwm>710)
					pwm=710;
			}
		}
		else if(analog2<1600)
		{
			if(selected==0)
			{
				p-=10/(1700/(1600-analog2));
				change=1;
				if(p<0)
					p=0;
			}
			else if(selected==1)
			{
				d-=10/(1700/(1600-analog2));
				change=1;
				if(d<0)
					d=0;
			}
			if(selected==2)
			{
				pwm-=10/(1700/(1600-analog2));
				change=1;
				if(pwm<0)
					pwm=0;
			}
		}
		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)==0)
		{
			esp8266_send_cmd("AT+CIPSEND=0,10","OK",50);
		  u3_printf("%s\r\n",ack);	//发送命令
			delay_ms(200);
		}
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==0)
		{
			esp8266_send_cmd("AT+CIPSEND=0,10","OK",50);
		  u3_printf("%s\r\n","0000000000");	//发送命令
			delay_ms(200);
		}
		
		if(change)
		{
			OLED_Clear();
		OLED_ShowString(0,0,"P",16);
		OLED_ShowNum(36,0,p,3,16);
	  OLED_ShowString(0,16,"I",16);
		OLED_ShowNum(36,16,d,3,16);
		OLED_ShowString(0,32,"PWM",16);
		OLED_ShowNum(36,32,pwm,3,16);
		if(selected==0)
			OLED_ShowString(76,0,"on",16);
		else if(selected==1)
			OLED_ShowString(76,16,"on",16);
		else if(selected==2)
			OLED_ShowString(76,32,"on",16);
	  OLED_Refresh();
		
		create_string(p,0);
		create_string(d,1);
		create_string(pwm,2);
		printf("%s\r\n",ack);
		}

			delay_ms(10);
		change=0;
	
 }

 
 
}

 /*
//OLED_ShowPicture(0,0,128,8,BMP1);
		delay_ms(500);
		OLED_Clear();
		OLED_ShowChinese(0,0,0,16);//中
		OLED_ShowChinese(18,0,1,16);//景
		OLED_ShowChinese(36,0,2,16);//园
		OLED_ShowChinese(54,0,3,16);//电
		OLED_ShowChinese(72,0,4,16);//子
		OLED_ShowChinese(90,0,5,16);//科
		OLED_ShowChinese(108,0,6,16);//技
		OLED_ShowString(8,16,"ZHONGJINGYUAN",16);
		OLED_ShowString(20,32,"2014/05/01",16);
		OLED_ShowString(0,48,"ASCII:",16);  
		OLED_ShowString(63,48,"CODE:",16);
		OLED_ShowChar(48,48,t,16);//显示ASCII字符	   
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,48,t,3,16);
		OLED_Refresh();
		delay_ms(500);
		OLED_Clear();
		OLED_ShowChinese(0,0,0,16);  //16*16 中
	  OLED_ShowChinese(16,0,0,24); //24*24 中
		OLED_ShowChinese(24,20,0,32);//32*32 中
	  OLED_ShowChinese(64,0,0,64); //64*64 中
		OLED_Refresh();
	  delay_ms(500);
*/
