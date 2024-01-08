/**********************BST-V51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52RD
*  名称：步进电机调速
*  公司：深圳市亚博软件开发有限公司      
*  晶振:11.0592MHZ
*  实验效果：本程序用于测试4相步进电机常规驱动 4个独立按键 分别控制加速、减速、开启、停止       
		数码管显示 01-18速度等级，数字越大，速度越大
******************************************************************/
#include <reg52.h>

#define dula P0		//段选信号的锁存器控制
sbit wei1=P2^4;
sbit wei2=P2^5;
sbit wei3=P2^6;
sbit wei4=P2^7;

unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

sbit A1=P1^0; //定义步进电机连接端口
sbit B1=P1^1;
sbit C1=P1^2;
sbit D1=P1^3;
sbit k1 = P3^4;
sbit k2 = P3^5;
sbit k3 = P3^6;
sbit k4 = P3^7;


#define Coil_CD1 {A1=1;B1=1;C1=0;D1=0;}//CD相通电，其他相断电
#define Coil_AD1 {A1=0;B1=1;C1=1;D1=0;}//AD相通电，其他相断电
#define Coil_AB1 {A1=0;B1=0;C1=1;D1=1;}//AB相通电，其他相断电
#define Coil_BC1 {A1=1;B1=0;C1=0;D1=1;}//BC相通电，其他相断电
#define Coil_A1 {A1=0;B1=1;C1=1;D1=1;}//A相通电，其他相断电
#define Coil_B1 {A1=1;B1=0;C1=1;D1=1;}//B相通电，其他相断电
#define Coil_C1 {A1=1;B1=1;C1=0;D1=1;}//C相通电，其他相断电
#define Coil_D1 {A1=1;B1=1;C1=1;D1=0;}//D相通电，其他相断电
#define Coil_OFF {A1=1;B1=1;C1=1;D1=1;}//全部断电

unsigned char Speed=1;
bit StopFlag;
void Display(unsigned char num);
void Init_Timer0(void);
/*------------------------------------------------
延时函数，毫秒级别
------------------------------------------------*/
void delay(unsigned int xms)				
{
	unsigned int i,j;
	for(i=xms;i>0;i--)		      //i=xms即延时约xms毫秒
		for(j=112;j>0;j--);
}


/*------------------------------------------------
                    主函数
------------------------------------------------*/
main()
{
 unsigned int i=512;//旋转一周时间
 //unsigned char num;
 Init_Timer0();
 Coil_OFF
 while(1)  //正向
  { 
   if(k1 == 0)//第一个按键,速度等级增加
	{
		delay(10);//延时10ms 消抖
		if(k1 == 0)//再次判断按键是否被按下
		{
			while(k1 == 0);//直到按键判断松开
			if(Speed<18)
				Speed++;	
		}
	}
	else if(k2 == 0)//第二个按键，速度等级减小
		{
			delay(10);//延时10ms 消抖
			if(k2 == 0)//再次判断按键是否被按下
			{
				while(k2 == 0);//直到按键判断松开
				if(Speed>1)
	   				Speed--;
			}
		}
	else if(k3 == 0)//第三个按键，暂停
		{
			Coil_OFF
  			StopFlag=1;
		}
	else if(k4 == 0)//第四个按键，开启
		StopFlag=0;
	Display(Speed);
  }
  
}

/*------------------------------------------------
 数码管显示函数，用于动态扫描数码管
------------------------------------------------*/
void Display(unsigned char num)
{
		unsigned char shi,ge;

		shi = num/10;
		ge = num%10;

		wei1=0;		//显示十位
		wei2=0;
		wei3=1;
		wei4=0;
		P0=table[shi];
		delay(2);
		
		wei1=0;		//显示个位
		wei2=0;
		wei3=0;
		wei4=1;
		P0=table[ge];
		delay(2);
}
/*------------------------------------------------
                    定时器初始化子程序
------------------------------------------------*/
void Init_Timer0(void)
{
 TMOD |= 0x01;	  //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响		     
 //TH0=0x00;	      //给定初值
 //TL0=0x00;
 EA=1;            //总中断打开
 ET0=1;           //定时器中断打开
 TR0=1;           //定时器开关打开
}
/*------------------------------------------------
                 定时器中断子程序
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 
{
 static unsigned char times,i;
 TH0=(65536-1000)/256;		  //重新赋值 1ms
 TL0=(65536-1000)%256;
 
 if(!StopFlag)
   {
  if(times==(20-Speed))//最大值18，所以最小间隔值20-18=2
   {
   times=0;
   switch(i)
       {
	    case 0:Coil_A1;i++;break;
		case 1:Coil_B1;i++;break;
		case 2:Coil_C1;i++;break;
		case 3:Coil_D1;i++;break;
		case 4:i=0;break;
	    default:break;
	   }
    }
 times++;
  }
}


