/**********************BST-M51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：24L01无线发射实验
*  公司：深圳市亚博软件开发有限公司       
*  日期：2015-7
*  晶振:11.0592MHZ
******************************************************************/
#include"reg52.h"
#include"nrf24l01.h"
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
/*********************主函数***************************/
void main()
{
   unsigned int Date;
   char TxDate[4];
   NRF24L01Int();
   while(1)
    {
		 TxDate[0]=table[1];
		 TxDate[1]=table[5];
		 TxDate[2]=table[6];
		 TxDate[3]=table[8];	
		 NRFSetTxMode(TxDate);
		 while(CheckACK());	//检测是否发送完毕
	}

}