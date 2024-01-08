/**********************BST-M51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：按键扫描
*  公司：深圳市亚博软件开发有限公司       
*  日期：2015-8
*  晶振:11.0592MHZ
******************************************************************/
#include <intrins.H>
void delayMs(unsigned int xms)				
{
	unsigned int i,j;
	for(i=xms;i>0;i--)		      //i=xms即延时约xms毫秒
		for(j=112;j>0;j--);
}

void delayUs()
{
    _nop_();
}