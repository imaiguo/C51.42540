/***************************************************
*ʵ��������ˮ�Ʊ���
*ʵ��Ч����������¼��ȥ�� �����������ˮ�Ʒ�������
*************************************************/
#include<reg52.h>
#include<intrins.h>

#define on 0
#define off 1
sbit fm = P2^3;

void delay(unsigned int xms)				
{
	unsigned int i,j;
	for(i=xms;i>0;i--)		      //i=xms����ʱԼxms����
		for(j=112;j>0;j--);
}

void main()
{
	P1 = 0xfe;//1111 1110
	while(1)
	{
		delay(100);
		P1 =_crol_(P1,1);/*��P1ѭ������1λ*/
		fm = on;
		delay(100);
		fm = off;
	}
}