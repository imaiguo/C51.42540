#include <reg51.h>
#include"Delay.h"

sbit dula=P2^6;		//段选信号的锁存器控制
sbit wela=P2^7;		//位选信号的锁存器控制


void shumaguan(unsigned char x)
{

			P0=x;	   
			dula=1;
			dula=0;

			P0=0xfe;	   //选中第一个数码管
			wela=1;
			wela=0;
			Delay(1000);
}

