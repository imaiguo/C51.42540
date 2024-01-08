/**********************BST-M51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：24L01无线接收（从机2）
*  公司：深圳市亚博软件开发有限公司       
*  日期：2015-6
*  晶振:11.0592MHZ
******************************************************************/
#include<reg52.h>
#include"NRF24L01.h"  
#include"Delay.h"


sbit led = P1^1;
//idata unsigned char TxDate[TX_DATA_WITDH]={3,3};			  //发送给主机的数据



void main()
{

   unsigned int num=0 ;   
   led = 1; 

   NRF24L01Int();  
   uart_init();   
  
   
   while(1)
    {  	   

 
	   NRFSetRXMode();   //设置为接收模式  
	   GetDate();        //开始接收数据 ，实时接收



 	
	}	
}