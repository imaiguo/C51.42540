/*************************************************
*ʵ������������������
*ʵ��Ч�������µ�1����������K1 2����ɫ��led����
		   ���µ�2����������K1 2����ɫ��led����
		   ���µ�3����������K1 2����ɫ��led����
		   ���µ�4����������K1 2����ɫ��led����
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

sbit k1 = P3^4;
sbit k2 = P3^5;
sbit k3 = P3^6;
sbit k4 = P3^7;

void main()
{
	P1 = 0xff;//Ϩ������led��
	while(1)
	{
	led1 = k1;
	led2 = k1;
	/*	if(k1 == 0)
		{
			led1 = 0;
			led2 = 0;
		}
		else
		{
			led1 = 1;
			led2 = 1;
		}
	*/
	led3 = k2;
	led4 = k2;
	led5 = k3;	
	led6 = k3;
	led7 = k4;
	led8 = k4;
	}
}