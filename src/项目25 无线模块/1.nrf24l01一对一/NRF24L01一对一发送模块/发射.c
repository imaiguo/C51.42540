/**********************BST-M51ʵ�鿪��������************************
*  ƽ̨��BST-M51 + Keil U4 + STC89C52
*  ���ƣ�24L01���߷���ʵ��
*  ��˾���������ǲ�����������޹�˾       
*  ���ڣ�2015-7
*  ����:11.0592MHZ
******************************************************************/
#include"reg52.h"
#include"nrf24l01.h"
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
/*********************������***************************/
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
		 while(CheckACK());	//����Ƿ������
	}

}