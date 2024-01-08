/**********************************************
*实验名： 蜂鸣器实验
*实验效果：程序烧录进去后 蜂鸣器发出滴滴的声音
**********************************************/
#include<reg52.h>

#define on 0
#define off 1
sbit fm = P2^3;

void delay(unsigned int xms)				
{
	unsigned int i,j;
	for(i=xms;i>0;i--)		      //i=xms即延
		for(j=112;j>0;j--);
}

void main()
{
	while(1)
	{
		fm = on;//P2^3 = 0;蜂鸣器响
		delay(100);
		fm = off;
		delay(100);
	}
}
