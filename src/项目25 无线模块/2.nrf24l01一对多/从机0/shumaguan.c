#include <reg51.h>
#include"Delay.h"

sbit dula=P2^6;		//��ѡ�źŵ�����������
sbit wela=P2^7;		//λѡ�źŵ�����������


void shumaguan(unsigned char x)
{

			P0=x;	   
			dula=1;
			dula=0;

			P0=0xfe;	   //ѡ�е�һ�������
			wela=1;
			wela=0;
			Delay(1000);
}

