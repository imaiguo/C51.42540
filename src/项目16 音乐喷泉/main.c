/**********************BST-M51ʵ�鿪��������************************
*  ƽ̨��BST-M51 + Keil U4 + STC89C52
*  ���ƣ�����ģ����
*  ��˾���������ǲ�����������޹�˾       
*  ���ڣ�2015-7
*  ����:11.0592MHZ
*  ʵ��Ч����������ʱ����⵽������led1��
*************************************************/
#include<reg52.h>

sbit led1 = P1^0;//RED
sbit led2 = P1^1;//RED
sbit led3 = P1^2;//GREEN
sbit led4 = P1^3;//GREEN
sbit led5 = P1^4;//YELLOW
sbit led6 = P1^5;//YELLOW
sbit led7 = P1^6;//BLUE
sbit led8 = P1^7;//BLUE

sbit voice = P2^0;

unsigned long level = 0;

void timer0_init()
{
	TMOD = 0x01; //��ʱ��0ѡ������ʽ1
    TH0 = 0xDC;	 //���ó�ʼֵ	10ms
    TL0 = 0x00; 
    EA = 1;			 //�����ж�
    ET0 = 1;		 //�򿪶�ʱ��0�ж�
    TR0 = 1;		 //������ʱ��0
}

void main()
{
	P1 = 0xff;//Ϩ������led��
	timer0_init();
	while(1)	//���ݲ�ͬ�ȼ��ж�led����
	{
		if(voice == 0)
		{
			led1 = voice;
		}
		if((led1 == 0)&&(level >= 1))
		{
			 led2 = 0;
		}
		if((led2 == 0)&&(level >= 2))
		{
			 led3 = 0;
		}
		if((led3 == 0)&&(level >= 3))
		{
			 led4 = 0;
		}
		if((led4 == 0)&&(level >= 4))
		{
			 led5 = 0;
		}
		if((led5 == 0)&&(level >= 5))
		{
			 led6 = 0;
		}
		if((led6 == 0)&&(level >= 6))
		{
			 led7 = 0;
		}
		if((led7 == 0)&&(level >= 7))
		{
			 led8 = 0;
		}
		if(level == 0)
		{
			P1 = 0xff;
		}
	}
}

void timer0() interrupt 1
{
    TH0 = 0xDC;	 //���ó�ʼֵ  10ms
    TL0 = 0x00; 
	if(voice == 0) //ÿ��10ms���һ�Σ���������������������ȼ�
		level++;
	else
		level = 0;
}