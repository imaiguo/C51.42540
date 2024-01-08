/**********************BST-M51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：计算器 
*  公司：深圳市亚博软件开发有限公司     
*  日期：2015-7
*  晶振:11.0592MHZ
******************************************************************
							按键说明
				键盘上数字0--9的输入		#：清零
				A：加   B：减  C：除	D：等于	
程序功能：	本程序为简易计算器。可以算整数且正数类型的运算。
******************************************************************/
#include<reg51.h>

#define dula P0		//段选信号的锁存器控制
#define wela P2		//位选信号的锁存器控制,这里只用到P2.4-P2.7

unsigned char temp,key=0,wei=0,i,j,k,keydown=0;
unsigned char jia=0,jian=0,cheng=0,chu=0,jia0=0,jian0=0,cheng0=0,chu0=0,dengyu=0,qingling=0;
unsigned char s0=16,s1=16,s2=16,s3=16; //参加运算的各个位
unsigned long qian = 0,hou = 0;//定义参于运算的第一个数和第二个数。

unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00,0x40};
unsigned char code weitable[]={0x8f,0x4f,0x2f,0x1f};
						//数码管各位的码表
void delay(unsigned char i)
{
	for(j=i;j>0;j--)
	for(k=125;k>0;k--);
}
void display1(unsigned char wei,unsigned char shu)//在任意一位显示任意的数字
{
   wei=wei-1;
   wela|=0xf0;//给P2.4-P2.7置1
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

void keyscan4x4()
{
    unsigned char temp;


	///////////////第一行扫描///////////////////
	P3=0xfe;//1111 1110 让P3.0口输出低
	temp=P3;
	temp=temp&0xf0;//1111 0000	位与操作 屏蔽后四位
	if(temp!=0xf0)
	{
	    delay(10);
		temp=P3;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
		    temp=P3;
			switch(temp)
			{
			  case 0xee:	   //1110 1110 "1"被按下
			       key=1;
				   break;
			  case 0xde:		  //1101 1110  "2"被按下
				   key=2;
				   break;
			  case 0xbe:		  //1011 1110  "3"被按下
				   key=3;
				   break;
			  case 0x7e:		  //0111 1110  "A"被按下
				   {key=10;jia=1;jian=0;cheng=0;chu=0;}
				   break;
			}
			while(temp!=0xf0)
			{
			    temp=P3;
				temp=temp&0xf0;
			}
			//display(key);
			keydown=1;
			wei++;
		}
	}
			///////////////第二行扫描///////////////////
	P3=0xfd;//1111 1101 让P3.1口输出低
	temp=P3;
	temp=temp&0xf0;//1111 0000	位与操作 屏蔽后四位
	if(temp!=0xf0)
	{
	    delay(10);
		temp=P3;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
		    temp=P3;
			switch(temp)
			{
			  case 0xed:	   //1110 1101 "4"被按下
			       key=4;
				   break;
			  case 0xdd:		  //1101 1101  "5"被按下
				   key=5;
				   break;
			  case 0xbd:		  //1011 1101  "6"被按下
				   key=6;
				   break;
			  case 0x7d:		  //0111 1101  "B"被按下
				   {key=11;jia=0;jian=1;cheng=0;chu=0;}
				   break;
			}
			while(temp!=0xf0)
			{
			    temp=P3;
				temp=temp&0xf0;
			}
			//display(key);
			keydown=1;
			wei++;
		}
	 }
			///////////////第三行扫描///////////////////
	P3=0xfb;//1111 1011 让P3.2口输出低
	temp=P3;
	temp=temp&0xf0;//1111 0000	位与操作 屏蔽后四位
	if(temp!=0xf0)
	{
	    delay(10);
		temp=P3;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
		    temp=P3;
			switch(temp)
			{
			  case 0xeb:	   //1110 1011 "7"被按下
			       key=7;
				   break;
			  case 0xdb:		  //1101 1011  "8"被按下
				   key=8;
				   break;
			  case 0xbb:		  //1011 1011  "9"被按下
				   key=9;
				   break;
			  case 0x7b:		  //0111 1011  "C"被按下
				   {key=12;jia=0;jian=0;cheng=0;chu=1;}
				   break;
			}
			while(temp!=0xf0)
			{
			    temp=P3;
				temp=temp&0xf0;
			}
			//display(key);
			keydown=1;
			wei++;
		  }
		}

			///////////////第四行扫描///////////////////
	P3=0xf7;//1111 0111 让P3.0口输出低
	temp=P3;
	temp=temp&0xf0;//1111 0000	位与操作 屏蔽后四位
	if(temp!=0xf0)
	{
	    delay(10);
		temp=P3;
		temp=temp&0xf0;
		if(temp!=0xf0)
		{
		    temp=P3;
			switch(temp)
			{
			  case 0xe7:	   //1110 0111 "*"被按下
			       {key=14;jia=0;jian=0;cheng=1;chu=0;}
				   break;
			  case 0xd7:		  //1101 0111  "0"被按下
				   key=0;
				   break;
			  case 0xb7:		  //1011 0111  "#"被按下
				   {key=15;qingling=1;}
				   break;
			  case 0x77:		  //0111 0111  "D"被按下
				   {key=13;dengyu =1;}
				   break;
			}
			while(temp!=0xf0)
			{
			    temp=P3;
				temp=temp&0xf0;
			}
			//display(key);
			keydown=1;
			wei++;
		 }
	 }

}

void main()
{	
	unsigned char gongneng = 0,yunsuan = 0,yunsuan0 = 0;
	wei = 0;

  	while(1)
	{
	  	keyscan4x4();
		gongneng=jia|jian|cheng|chu|jia0|jian0|cheng0|chu0|dengyu|qingling;
		if(gongneng==0)
		{		
			if((!keydown)&&(!wei))
			{
			   s0=16;s1=16;s2=16;s3=16;
			   qian = 0;
			}
			if((keydown)&&(key<10))
			{
				if(wei == 1)
				{s3=key;qian=s3;}
				else if(wei == 2)
				{
					s2=s3;
					s3=key;
					qian=s2*10+s3;
				}
				else if(wei == 3)
				{
					s1=s2;
					s2=s3;
					s3=key;
					qian=s1*100+s2*10+s3;
				}
				else if(wei == 4)
				{	
					s0=s1;
					s1=s2;
					s2=s3;
					s3=key;
					qian=s0*1000+s1*100+s2*10+s3;
				}
				keydown = 0;
			}	
		}
		else
		{  	
			yunsuan=jia|jian|cheng|chu;
			yunsuan0=jia0|jian0|cheng0|chu0;
			
			if((yunsuan)&&(key > 9)&&(!yunsuan0)&&(!hou))
			{
				wei = 0;
				keydown = 0;
				s0=16;s1=16;s2=16;s3=16;
				if(jia)
				{
					jia=0;	jia0=1;jian0=0;cheng0=0;chu0=0;
				}
				if(jian)
				{
					jian=0;jia0=0;jian0=1;cheng0=0;chu0=0;
				}
				if(cheng)
				{
					cheng=0;jia0=0;jian0=0;cheng0=1;chu0=0;
				}
				if(chu)
				{
					chu=0;	jia0=0;jian0=0;cheng0=0;chu0=1;
				}
			}
			else if((yunsuan)&&(key > 9)&&(!yunsuan0))
			{
				wei = 0;
				keydown = 0;
				if(jia)
				{
					jia=0;	jia0=1;jian0=0;cheng0=0;chu0=0;
				}
				if(jian)
				{
					jian=0;jia0=0;jian0=1;cheng0=0;chu0=0;
				}
				if(cheng)
				{
					cheng=0;jia0=0;jian0=0;cheng0=1;chu0=0;
				}
				if(chu)
				{
					chu=0;	jia0=0;jian0=0;cheng0=0;chu0=1;
				}
				dengyu = 0;
			}
			else if(((yunsuan)&&(key > 9)&&(yunsuan0))||(dengyu))
			{
				if(jia0)
				{
					hou=qian+hou;
					jia0=0;
				}
				if(jian0)
				{
					hou=qian-hou;
					jian0=0;
				}
				if(cheng0)
				{
					hou=qian*hou;
					cheng0=0;
				}
				if(chu0)
				{
					hou=qian/hou;
					chu0=0;
				}

				if((hou < 0)||(hou > 10000))
				{s3=17;s2=17;s1=17;s0=17;hou=0;}
				else if(hou<10)
				{s3=hou;s2=16;s1=16;s0=16;}
				else if(hou<100)
				{s3=hou%10;s2=hou/10;s1=16;s0=16;}
				else if(hou<1000)
				{s3=hou%100%10;s2=hou%100/10;s1=hou/100;s0=16;}
				else if(hou<10000)
				{s3=hou%1000%100%10;s2=hou%1000%100/10;s1=hou%1000/100;s0=hou/1000;}
				qian = hou;
				wei = 0;
				keydown = 0;
			}
			else if((yunsuan0)&&(key < 10))
			{	
				if((!keydown)&&(!wei))
				{
				   s0=16;s1=16;s2=16;s3=16;
				   hou = 0;
				}
				if(keydown)
				{
					if(wei == 1)
					{
						s0=16;s1=16;s2=16;s3=16;
						s3=key;hou=s3;
					}
					else if(wei == 2)
					{
						s2=s3;
						s3=key;
						hou=s2*10+s3;
					}
					else if(wei == 3)
					{
						s1=s2;
						s2=s3;
						s3=key;
						hou=s1*100+s2*10+s3;
					}
					else if(wei == 4)
					{	
						s0=s1;
						s1=s2;
						s2=s3;
						s3=key;
						hou=s0*1000+s1*100+s2*10+s3;
					}
					keydown = 0;
				}
			}

			if(qingling)
			{
				s0=16;s1=16;s2=16;s3=16;
				jia=0;jian=0;cheng=0;chu=0;
				jia0=0;jian0=0;cheng0=0;chu0=0;
				qingling=0;dengyu=0;wei=0;
				qian=0;hou=0;
			}			
		}
		display(s0,s1,s2,s3);
	}

}