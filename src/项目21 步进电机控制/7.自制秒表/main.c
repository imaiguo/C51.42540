/**********************BST-V51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52RD
*  名称：自制秒表
*  公司：深圳市亚博软件开发有限公司      
*  晶振:11.0592MHZ
*  实验效果：利用步进电机转角的精准制作圆形秒表
******************************************************************/
#include <reg52.h>

#define dula P0		//段选信号的锁存器控制
sbit wei1=P2^4;
sbit wei2=P2^5;
sbit wei3=P2^6;
sbit wei4=P2^7;
sbit fm = P2^3;

unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

sbit A1=P1^0; //定义步进电机连接端口
sbit B1=P1^1;
sbit C1=P1^2;
sbit D1=P1^3;

#define Coil_AB1 {A1=0;B1=0;C1=1;D1=1;}//AB相通电，其他相断电
#define Coil_BC1 {A1=1;B1=0;C1=0;D1=1;}//BC相通电，其他相断电
#define Coil_CD1 {A1=1;B1=1;C1=0;D1=0;}//CD相通电，其他相断电
#define Coil_DA1 {A1=0;B1=1;C1=1;D1=0;}//D相通电，其他相断电
#define Coil_A1 {A1=0;B1=1;C1=1;D1=1;}//A相通电，其他相断电
#define Coil_B1 {A1=1;B1=0;C1=1;D1=1;}//B相通电，其他相断电
#define Coil_C1 {A1=1;B1=1;C1=0;D1=1;}//C相通电，其他相断电
#define Coil_D1 {A1=1;B1=1;C1=1;D1=0;}//D相通电，其他相断电
#define Coil_OFF {A1=1;B1=1;C1=1;D1=1;}//全部断电

unsigned int count=0;
bit StopFlag;
void display(unsigned char num);
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
 unsigned int time=0;
 Init_Timer0();
 fm = 1;
 Coil_OFF
 while(1)
 {
	 if(count == 68)
	 {
	 	count = 0;
		time++;
		if(time == 60)
			time = 0;
		fm = 0;
		delay(10);
		fm =1;
	 }
	 display(time);
 }
}

/*------------------------------------------------
 数码管显示函数，用于动态扫描数码管
------------------------------------------------*/
void display(unsigned char num)
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
 TH0=0xcb;		  //重新赋值 1/68s
 TL0=0x0f;
 EA=1;            //总中断打开
 ET0=1;           //定时器中断打开
 TR0=1;           //定时器开关打开
}
/*------------------------------------------------
                 定时器中断子程序
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 
{
 static unsigned char i;
 TH0=0xcb;		  //重新赋值 1/68s
 TL0=0x0f;
 
   switch(i)
       {
	    case 0:Coil_A1;i++;break;
		case 1:Coil_AB1;i++;break;
		case 2:Coil_B1;i++;break;
		case 3:Coil_BC1;i++;break;
		case 4:Coil_C1;i++;break;
		case 5:Coil_CD1;i++;break;
		case 6:Coil_D1;i++;break;
		case 7:Coil_DA1;i=0;break;
	    default:break;
	   }
 count++;
}


