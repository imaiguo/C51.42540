/**********************BST-M51ʵ�鿪��������************************
*  ƽ̨��BST-M51 + Keil U4 + STC89C52
*  ���ƣ�24L01���߽��գ��ӻ�2��
*  ��˾���������ǲ�����������޹�˾       
*  ���ڣ�2015-6
*  ����:11.0592MHZ
******************************************************************/
#include<reg52.h>
#include"NRF24L01.h"  
#include"Delay.h"


sbit led = P1^1;
//idata unsigned char TxDate[TX_DATA_WITDH]={3,3};			  //���͸�����������



void main()
{

   unsigned int num=0 ;   
   led = 1; 

   NRF24L01Int();  
   uart_init();   
  
   
   while(1)
    {  	   

 
	   NRFSetRXMode();   //����Ϊ����ģʽ  
	   GetDate();        //��ʼ�������� ��ʵʱ����



 	
	}	
}