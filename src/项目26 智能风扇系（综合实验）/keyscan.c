/**********************BST-M51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：按键扫描
*  公司：深圳市亚博软件开发有限公司       
*  日期：2015-8
*  晶振:11.0592MHZ
******************************************************************/
#include<reg51.h>
#include<allhead.h>

unsigned char keyscan()
{
    static unsigned char temp,key;

	///////////////第四列扫描///////////////////
	P3 &= 0xfe;//1111 1110 让P3.0口输出低
	temp=P3;
	temp=temp&0xf0;//1111 0000	位与操作 屏蔽后四位
	if(temp!=0xf0)
	{
	    delayMs(10);//消抖
		temp=P3;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
		    temp=P3;
			switch(temp)
			{
			  case 0x7e:	   //1110 1110 "A"被按下
			       key=level1;
				   break;
			  case 0xbe:		  //1101 1110  "B"被按下
				   key=level2;
				   break;
			  case 0xde:		  //1011 1110  "C"被按下
				   key=level3;
				   break;
			  case 0xee:		  //0111 1110  "D"被按下
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