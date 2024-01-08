/**********************BST-M51ʵ�鿪��������************************
*  ƽ̨��BST-M51 + Keil U4 + STC89C52
*  ���ƣ����
*  ��˾���������ǲ�����������޹�˾      
*  ���ڣ�2015-6
*  ����:11.0592MHZ
*  ˵������ѿ�Դ�����ṩԴ�������.
******************************************************************/

//ע�⾧����Ϊ11.0592M��
//��Ϊ������ֵ������ı�TH0��TL0����ֵ�������ʱ���кܴ���� ��

#include<reg51.h>

#define uchar unsigned char

#define dula P0		//��ѡ�źŵ�����������
#define wela P2		//λѡ�źŵ�����������,����ֻ�õ�P2.4-P2.7

sbit k4 = P3^6;//��ɫ����k4
sbit k5 = P3^7;//��ɫ����k5
sbit beep = P2^3;//������

bit stop = 1;
unsigned char j,k,num0,num1,num2,num3,sec,min,count=0;
unsigned char time_count;
unsigned char code weitable[]={0x8f,0x4f,0x2f,0x1f};
//����ܸ�λ�����
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//����ܸ�λ�����(����С��)
unsigned char code table1[]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,
                        0x87,0xff,0xef};
void delay(unsigned char i)
{
  for(j=i;j>0;j--)
    for(k=125;k>0;k--);
}

void display1(uchar wei,uchar shu)//������һλ��ʾ���������
{
   wei=wei-1;
   wela|=0xf0;//��P2.4-P2.7��1
   if(wei == 2)
   	P0=table1[shu];
   else
    P0=table[shu];
   wela=wela&weitable[wei];//��P2��Ҫ��ʾ����һλ��1��������0
   delay(5);
}
void display(unsigned char a,unsigned char b,unsigned char c,unsigned char d)
{						  //һ����ʾ4�����֣���ÿλ��ʾ���ֿ��Զ���
   display1(4,a);
   display1(3,b);
   display1(2,c);
   display1(1,d);
}

void start_timer()
{
   	ET0=1;			 //����ʱ��0�ж�
	EA=1;			 //�����ж�
	TR0=1;			 //�򿪶�ʱ��
}

void stop_timer()
{
   	ET0=0;			 //�ض�ʱ��0�ж�
	EA=0;			 //�����ж�
	TR0=0;			 //�رն�ʱ��
}

void main()
{
	TMOD=0x01;		 //ģʽ���ã�00000001���ɼ����õ��Ƕ�ʱ��0��������ģʽ1��M1=0��M0=1����
	TH0=(65536-46080)/256;// ���ھ���Ϊ11.0592,�����Ǵ���ӦΪ46080����ʱ��ÿ��50000΢�뷢��һ���жϡ�
	TL0=(65536-46080)%256;//46080��������Ϊ50000*11.0592/12
	while(1)
	{
		if(k4 == 0)//�ж��Ƿ��а��°������ź�
		{
			delay(10);//��ʱ10ms ����
			if(k4 == 0)//�ٴ��жϰ����Ƿ񱻰���
			{
				while(k4 == 0);//ֱ�������ж��ɿ�
				{
					if(stop)
						start_timer();			 //�򿪶�ʱ��
					else	
						stop_timer();			 //�رն�ʱ��
					stop = ~stop;
					beep = 1;
					P1 = 0xff;
				}
			}
		}
		else if(k5 == 0)//�ж��Ƿ��а��°������ź�
		{
			delay(10);//��ʱ10ms ����
			if(k5 == 0)//�ٴ��жϰ����Ƿ񱻰���
			{
				while(k5 == 0);//ֱ�������ж��ɿ�
				{	
					stop_timer();			 //�رն�ʱ��
					stop = 1;
					beep = 1;
					P1 = 0xff;
					count = 0;
					time_count = 0;
					sec = 0;			  //��ʱ����
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
		else if(time_count==20)//����20��0.05��Ϊһ��
		{
			count = 0;
			time_count=0;
			sec++;
			if(sec==60)
			{
				sec=0;	   //������60s�������
				min++;
			}
			if(min==60)
			{
				min=0;	   //������60min�������
			}
			beep = 0;
			delay(10);
			beep = 1;
		}
		num0=sec%10;		  //ȡ����ǰ�����ĸ�λ��ʮλ
		num1=sec/10;
		num2=min%10;		  //ȡ����ǰ�����ĸ�λ��ʮλ
		num3=min/10;
		display(num3,num2,num1,num0);	   //��ʾ
	}
}

void timer0() interrupt 1
{
	TH0=(65536-46080)/256;
	TL0=(65536-46080)%256;
	time_count++;
}