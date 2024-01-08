/**********************BST-M51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：24L01无线发射（一对多）
*  公司：深圳市亚博软件开发有限公司       
*  日期：2015-6
*  晶振:11.0592MHZ
******************************************************************/
#include<reg52.h>
#include"NRF24L01.h"  
#include"Delay.h"

sbit  beep=P2^3;
sbit  led=P1^0;
sbit  key1=P3^4;
sbit  key2=P3^5;
sbit  key3=P3^6;
sbit  key4=P3^7;

//unsigned int num;

idata unsigned char TxDate0[TX_DATA_WITDH]={0x3f};			  //发送给主机的数据	 数码管 0
idata unsigned char TxDate1[TX_DATA_WITDH]={0x06};			  //发送给主机的数据	 数码管 1
idata unsigned char TxDate2[TX_DATA_WITDH]={0x5b};			  //发送给主机的数据	 数码管 2
idata unsigned char TxDate3[TX_DATA_WITDH]={0x4f};			  //发送给主机的数据 	 数码管 3
//idata unsigned char TxDate4[TX_DATA_WITDH]={0x66};			  //发送给主机的数据	 数码管 4
//idata unsigned char TxDate5[TX_DATA_WITDH]={0x6d};			  //发送给主机的数据	 数码管 5

void main()
{

 // num =0 ;   
   led = 1; 

   NRF24L01Int();     
  
   
   while(1)
    {  	   
	   if(key1 == 0)									  //KEY1按下，等待发送数据给从机0
	      {
		       Delay(20);
			   if(key1==0)
			   {
                 Tx_Mode();
 		         NRFSetTxMode0(TxDate0);  //发送给接收机0
		         while(CheckACK());	      //检测是否发送完毕,检测应答信号 				    	
		       } 
	 	   }

 
		 if(key2 == 0)									  //KEY2按下，等待发送数据给从机1
	      {
		       Delay(20);
			   if(key2==0)
			   {
                 Tx_Mode();
 		         NRFSetTxMode1(TxDate1);  //发送给接收机1
		         while(CheckACK());	      //检测是否发送完毕,检测应答信号 				    	
		       } 
	 	   }

	 if(key3 == 0)									  //KEY3按下，等待发送数据给从机2
	      {
		       Delay(20);
			   if(key3==0)
			   {
                 Tx_Mode();
 		         NRFSetTxMode2(TxDate2);  //发送给接收机2
		         while(CheckACK());	      //检测是否发送完毕,检测应答信号 				    	
		       } 
	 	   }

      if(key4 == 0)									  //KEY4按下，等待发送数据给从机3
	      {
		       Delay(20);
			   if(key4 ==0)
			   {
                 Tx_Mode();
 		         NRFSetTxMode3(TxDate3);  //发送给接收机3
		         while(CheckACK());	      //检测是否发送完毕,检测应答信号 				    	
		       } 
	 	   }
 
 	
	}	
}