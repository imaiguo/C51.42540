/**********************BST-M51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：秒表
*  公司：深圳市亚博软件开发有限公司      
*  日期：2015-6
*  晶振:11.0592MHZ
*  说明：免费开源，不提供源代码分析.
******************************************************************/

//注意晶振需为11.0592M。
//若为其他数值晶振，请改变TH0与TL0参数值，否则计时会有很大误差 。

#include<reg51.h>

#define uchar unsigned char

#define dula P0		//段选信号的锁存器控制
#define wela P2		//位选信号的锁存器控制,这里只用到P2.4-P2.7

sbit k4 = P3^6;//红色按键k4
sbit k5 = P3^7;//红色按键k5
sbit beep = P2^3;//蜂鸣器

bit stop = 1;
unsigned char j,k,num0,num1,num2,num3,sec,min,count=0;
unsigned char time_count;
unsigned char code weitable[]={0x8f,0x4f,0x2f,0x1f};
//数码管各位的码表
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//数码管各位的码表(带上小点)
unsigned char code table1[]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,
                        0x87,0xff,0xef};
void delay(unsigned char i)
{
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}

void display1(uchar wei,uchar shu)//在任意一位显示任意的数字
{
   wei=wei-1;
   wela|=0xf0;//给P2.4-P2.7置1
   if(wei == 2)
   	P0=table1[shu];
   else
    P0=table[shu];
   wela=wela&weitable[wei];//给P2需要显示的那一位置1，其他置0
   delay(5);
}
void display(unsigned char a,unsigned char b,unsigned char c,unsigned char d)
{						  //一次显示4个数字，且每位显示数字可自定。
   display1(4,a);
   display1(3,b);
   display1(2,c);
   display1(1,d);
}

void start_timer()
{
   	ET0=1;			 //开定时器0中断
	EA=1;			 //开总中断
	TR0=1;			 //打开定时器
}

void stop_timer()
{
   	ET0=0;			 //关定时器0中断
	EA=0;			 //关总中断
	TR0=0;			 //关闭定时器
}

void main()
{
	TMOD=0x01;		 //模式设置，00000001，可见采用的是定时器0，工作与模式1（M1=0，M0=1）。
	TH0=(65536-46080)/256;// 由于晶振为11.0592,故所记次数应为46080，计时器每隔50000微秒发起一次中断。
	TL0=(65536-46080)%256;//46080的来历，为50000*11.0592/12
	while(1)
	{
		if(k4 == 0)//判断是否有按下按键的信号
		{
			delay(10);//延时10ms 消抖
			if(k4 == 0)//再次判断按键是否被按下
			{
				while(k4 == 0);//直到按键判断松开
				{
					if(stop)
						start_timer();			 //打开定时器
					else	
						stop_timer();			 //关闭定时器
					stop = ~stop;
					beep = 1;
					P1 = 0xff;
				}
			}
		}
		else if(k5 == 0)//判断是否有按下按键的信号
		{
			delay(10);//延时10ms 消抖
			if(k5 == 0)//再次判断按键是否被按下
			{
				while(k5 == 0);//直到按键判断松开
				{	
					stop_timer();			 //关闭定时器
					stop = 1;
					beep = 1;
					P1 = 0xff;
					count = 0;
					time_count = 0;
					sec = 0;			  //计时清零
					min = 0;
				}
			}
		}

		if(stop)
		{
			count++;
			if(count == 100)
			{
				P1 = ~P1;
				count = 0;
			}
		}
		else if(time_count==20)//计数20次0.05秒为一秒
		{
			count = 0;
			time_count=0;
			sec++;
			if(sec==60)
			{
				sec=0;	   //若到了60s，则归零
				min++;
			}
			if(min==60)
			{
				min=0;	   //若到了60min，则归零
			}
			beep = 0;
			delay(10);
			beep = 1;
		}
		num0=sec%10;		  //取出当前描述的个位与十位
		num1=sec/10;
		num2=min%10;		  //取出当前描述的个位与十位
		num3=min/10;
		display(num3,num2,num1,num0);	   //显示
	}
}

void timer0() interrupt 1
{
	TH0=(65536-46080)/256;
	TL0=(65536-46080)%256;
	time_count++;
}