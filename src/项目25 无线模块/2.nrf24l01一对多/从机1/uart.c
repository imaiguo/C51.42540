/**********************BST-M51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：24L01无线接收（从机1）
*  公司：深圳市亚博软件开发有限公司       
*  日期：2015-6
*  晶振:11.0592MHZ
******************************************************************/
#include<reg52.h>
#include"uart.h"
#include"delay.h"
 
sbit led = P1^0;


void uart_init()
{


	TMOD=0x20;      //设置计数器1的工作方式2x
	TH1=0xfd;			//设置计数器1的初值，决定波特率
	TL1=0xfd;			//设置计数器1的初值，决定波特率
	PCON=0x00;      // 波特率倍增0x00不加倍	 0x80加倍
	SCON=0x50;		//设置工作方式1 开启接受允许
	EA=1;		    //开启总中断
	ES=1;			//开启串口接受中断
	TR1=1;			//计数器1开始运行
}




void uart( unsigned int t )
{ 
 

/*	SCON = 0x40;  //方式1,不允许接收
	PCON = 0x80;  //波特率倍速

	TMOD = 0x20;  //定时器1方式2
//	TH1 = 256 - F_OSC / 16 / 12 / F_BAUD;//12MHz 
	TH1 = 0xfd ;   //9600
	TR1 = 1;//启动定时器1
*/


  	 

		SBUF = t ;
		while(!TI);
	    TI = 0;//完成上一次的发送
	  
	    
}
