/************************************
*实验名：流水灯
*实验效果：流水灯（移位操作）
*************************************/
#include<reg52.h>

void delay(unsigned int xms);

void main()
{
	int i=0;
	while(1)
	{
		P1 = 0xfe;//1111 1110
		for(i=0;i<8;i++)
		{
			delay(1000);
			P1 <<= 1;//P1 = P1<<1;
			P1 = P1 | 0x01;//	
		}
	}

}

void delay(unsigned int xms)//延时x毫秒				
{
	unsigned int i,j;
	for(i=xms;i>0;i--)		      //i=xms即延时约xms毫秒
		for(j=112;j>0;j--);
}