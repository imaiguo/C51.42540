/**********************BST-M51ʵ�鿪��������************************
*  ƽ̨��BST-M51 + Keil U4 + STC89C52
*  ���ƣ�����ɨ��
*  ��˾���������ǲ�����������޹�˾       
*  ���ڣ�2015-8
*  ����:11.0592MHZ
******************************************************************/
#include<reg51.h>
#include<allhead.h>

unsigned char keyscan()
{
    static unsigned char temp,key;

	///////////////������ɨ��///////////////////
	P3 &= 0xfe;//1111 1110 ��P3.0�������
	temp=P3;
	temp=temp&0xf0;//1111 0000	λ����� ���κ���λ
	if(temp!=0xf0)
	{
	    delayMs(10);//����
		temp=P3;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
		    temp=P3;
			switch(temp)
			{
			  case 0x7e:	   //1110 1110 "A"������
			       key=level1;
				   break;
			  case 0xbe:		  //1101 1110  "B"������
				   key=level2;
				   break;
			  case 0xde:		  //1011 1110  "C"������
				   key=level3;
				   break;
			  case 0xee:		  //0111 1110  "D"������
				   key=turnoff;
				   break;
			}
			while(temp!=0xf0)
			{
			    temp=P3;
				temp=temp&0xf0;
			}
			return key;  
		}		
	}
	else 
	return nokey;
}