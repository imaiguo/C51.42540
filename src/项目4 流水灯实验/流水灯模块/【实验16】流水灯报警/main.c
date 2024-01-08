/**********************BST-M51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：流水灯模块实验
*  公司：深圳市亚博软件开发有限公司      
*  日期：2015-6
*  晶振:11.0592MHZ
*  说明：免费开源，不提供源代码分析.
******************************************************************/
#include <reg51.h>
#include <intrins.h>
unsigned char a,b,k,j;
sbit beep=P2^3;
void delay10ms()
  {
    for(a=100;a>0;a--)
      for(b=225;b>0;b--);
   }
void main()
{
  k=0xfe;
  while(1)
  {
      delay10ms();
      
      beep=0;
      delay10ms();
      beep=1;
      j=_crol_(k,1);
      k=j;
      P1=j;
   }
}

