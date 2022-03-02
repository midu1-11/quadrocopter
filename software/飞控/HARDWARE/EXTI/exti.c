/***********************************************
公司：东莞市微宏智能科技有限公司
品牌：WHEELTEC
官网：wheeltec.net
淘宝店铺：shop114407458.taobao.com 
速卖通: https://minibalance.aliexpress.com/store/4455017
版本：5.7
修改时间：2021-04-29

Company: WeiHong Co.Ltd
Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version:5.7
Update：2021-04-29

All rights reserved
***********************************************/
#include "exti.h"
#include "MPU6050.h"
#include "usart.h"	
#include "adc.h"
#include "esp8266.h"


int anglei=0;
int anglek=0;
int anglej=0;
/**************************************************************************
Function: External interrupt initialization
Input   : none
Output  : none
函数功能：外部中断初始化
入口参数：无
返回  值：无 
**************************************************************************/
void MiniBalance_EXTI_Init(void)
{  
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);   //外部中断，需要使能AFIO时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能GPIO端口时钟
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	          //端口配置
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //上拉输入
		GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIO
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
		NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
}
//关闭INT中断
void NVIC_Close_Configuration(void)
{
	  NVIC_InitTypeDef NVIC_InitStructure;
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;								//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);
}
//开启INT中断
void NVIC_Open_Configuration(void)
{
	  NVIC_InitTypeDef NVIC_InitStructure;
	  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);
}

int EXTI1_IRQHandler(void) 
{
	/*
   float *p;	
	printf("exit!\r\n");
	 if(INT==0)		
	 {   
			EXTI->PR=1<<12;                         //清除中断标志位 
      p = Read_DMP();		 
		  anglei=p[0];	
		  anglek=p[1];
      anglej=p[2];		 
	 }   */    	
	 return 0;	  
}


