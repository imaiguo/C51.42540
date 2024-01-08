/**********************BST-M51ʵ�鿪��������************************
*  ƽ̨��BST-M51 + Keil U4 + STC89C52
*  ���ƣ�24L01���߷��䣨һ�Զࣩ
*  ��˾���������ǲ�����������޹�˾       
*  ���ڣ�2015-6
*  ����:11.0592MHZ
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

idata unsigned char TxDate0[TX_DATA_WITDH]={0x3f};			  //���͸�����������	 ����� 0
idata unsigned char TxDate1[TX_DATA_WITDH]={0x06};			  //���͸�����������	 ����� 1
idata unsigned char TxDate2[TX_DATA_WITDH]={0x5b};			  //���͸�����������	 ����� 2
idata unsigned char TxDate3[TX_DATA_WITDH]={0x4f};			  //���͸����������� 	 ����� 3
//idata unsigned char TxDate4[TX_DATA_WITDH]={0x66};			  //���͸�����������	 ����� 4
//idata unsigned char TxDate5[TX_DATA_WITDH]={0x6d};			  //���͸�����������	 ����� 5

void main()
{

 // num =0 ;   
   led = 1; 

   NRF24L01Int();     
  
   
   while(1)
    {  	   
	   if(key1 == 0)									  //KEY1���£��ȴ��������ݸ��ӻ�0
	      {
		       Delay(20);
			   if(key1==0)
			   {
                 Tx_Mode();
 		         NRFSetTxMode0(TxDate0);  //���͸����ջ�0
		         while(CheckACK());	      //����Ƿ������,���Ӧ���ź� 				    	
		       } 
	 	   }

 
		 if(key2 == 0)									  //KEY2���£��ȴ��������ݸ��ӻ�1
	      {
		       Delay(20);
			   if(key2==0)
			   {
                 Tx_Mode();
 		         NRFSetTxMode1(TxDate1);  //���͸����ջ�1
		         while(CheckACK());	      //����Ƿ������,���Ӧ���ź� 				    	
		       } 
	 	   }

	 if(key3 == 0)									  //KEY3���£��ȴ��������ݸ��ӻ�2
	      {
		       Delay(20);
			   if(key3==0)
			   {
                 Tx_Mode();
 		         NRFSetTxMode2(TxDate2);  //���͸����ջ�2
		         while(CheckACK());	      //����Ƿ������,���Ӧ���ź� 				    	
		       } 
	 	   }

      if(key4 == 0)									  //KEY4���£��ȴ��������ݸ��ӻ�3
	      {
		       Delay(20);
			   if(key4 ==0)
			   {
                 Tx_Mode();
 		         NRFSetTxMode3(TxDate3);  //���͸����ջ�3
		         while(CheckACK());	      //����Ƿ������,���Ӧ���ź� 				    	
		       } 
	 	   }
 
 	
	}	
}