/**********************BST-M51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：智能风扇
*  公司：深圳市亚博软件开发有限公司       
*  日期：2015-8
*  晶振:11.0592MHZ
******************************************************************/
#include<reg51.h>
#include<allhead.h>

#define Coil_CD1 {A1=1;B1=1;C1=0;D1=0;}//CD相通电，其他相断电
#define Coil_AD1 {A1=0;B1=1;C1=1;D1=0;}//AD相通电，其他相断电
#define Coil_AB1 {A1=0;B1=0;C1=1;D1=1;}//AB相通电，其他相断电
#define Coil_BC1 {A1=1;B1=0;C1=0;D1=1;}//BC相通电，其他相断电
#define Coil_A1 {A1=0;B1=1;C1=1;D1=1;}//A相通电，其他相断电
#define Coil_B1 {A1=1;B1=0;C1=1;D1=1;}//B相通电，其他相断电
#define Coil_C1 {A1=1;B1=1;C1=0;D1=1;}//C相通电，其他相断电
#define Coil_D1 {A1=1;B1=1;C1=1;D1=0;}//D相通电，其他相断电
#define Coil_OFF {A1=1;B1=1;C1=1;D1=1;}//全部断电

sbit A1=P1^4; //定义步进电机连接端口
sbit B1=P1^5;
sbit C1=P1^6;
sbit D1=P1^7;
sbit IR=P3^2;  //红外接口标志

unsigned char speed=0,key0=0,adda,key,irtime,irdata[33],IRcord[4];
int tmp;
unsigned int count=0;
bit irpro_ok,irok;
/*------------------------------------------------
                红外码值处理
------------------------------------------------*/
void Ircordpro(void)//红外码值处理函数
{ 
  unsigned char i, j, k;
  unsigned char cord,value;

  k=1;
  for(i=0;i<4;i++)      //处理4个字节
     {
      for(j=1;j<=8;j++) //处理1个字节8位
         {
          cord=irdata[k];
          if(cord>7)//大于某值为1，这个和晶振有绝对关系，这里使用12M计算，此值可以有一定误差
             value|=0x80;
          if(j<8)
		    {
			 value>>=1;
			}
           k++;
         }
     IRcord[i]=value;
     value=0;     
     } 
	 irpro_ok=1;//处理完毕标志位置1
}
/*------------------------------------------------
                  键值处理
------------------------------------------------*/

unsigned char Ir_work(void)//红外键值散转程序
{
	unsigned char key;
	switch(IRcord[2])//判断第三个数码值
	{
		 case 0x0c:key=level1;break;//1 显示相应的按键值
		 case 0x18:key=level2;break;//2
		 case 0x5e:key=level3;break;//3
	     default:break;
	}	
	irpro_ok=0;//处理完成标志
	return key;
}
/*------------------------------------------------
                    定时器0初始化子程序
------------------------------------------------*/
void timer0_init(void)
{
 TMOD |= 0x01;	  //定时器0工作方式2，TH0是重装值，TL0是初值，使用"|"符号可以在使用多个定时器时不受影响		     
 TH0=0xFC;	      //给定初值1ms
 TL0=0x66;
 EA=1;            //总中断打开
 ET0=1;           //定时器中断打开
 TR0=1;           //定时器开关打开
}
/*------------------------------------------------
                    定时器1初始化子程序
------------------------------------------------*/
void timer1_init(void)
{
 TMOD |= 0x01;	  //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响		     
 TH1=0xFC;	      //给定初值1ms
 TL1=0x66;
 EA=1;            //总中断打开
 ET1=1;           //定时器中断打开
 TR1=1;           //定时器开关打开
}
/*------------------------------------------------
                  外部中断0初始化
------------------------------------------------*/
void ex0init(void)
{
 IT0 = 1;   //指定外部中断0下降沿触发，INT0 (P3.2)
 EX0 = 1;   //使能外部中断
 EA = 1;    //开总中断
}
/*------------------------------------------------
                 主函数
------------------------------------------------*/
void main()
{
	timer0_init();
	timer1_init();
	Coil_OFF
	lcd_init();
	sendChangeCmd();
	while(1)
	{
		key0 = keyscan();

		if(key0 != nokey)
		{
			speed = key0;
			key = key0;
		}
		PCF8591_SendByte(2);
	 	adda=PCF8591_RcvByte();			 
		if((adda>150)&&((key0 == turnoff)||(key0 == nokey)))
		 {
			if(tmp>340)
				speed=level3;//t=5;
			else if(tmp>320)
				speed=level2; //t=6;
			else if(tmp>300)
			speed=level1;//t=7;			
		 }
		 else if((!key)&&(!adda))
		 	speed=turnoff;		 		
	}
	
}

/*------------------------------------------------
                 定时器1中断子程序
------------------------------------------------*/
void Timer1_isr(void) interrupt 3 
{
 static unsigned char times,i;
 unsigned char t;
 TH1=0xFC;		  //重新赋值 1ms
 TL1=0x66;
 
 switch(speed)
 {
	case level1:t=40;break;
	case level2:t=30;break;
	case level3:t=20;break;
	default:break;
 }
 if((speed)||(adda>150))
 {
	if(times==t)
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
/*------------------------------------------------
                 定时器0中断子程序
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 
{
 static unsigned char tt;
 TH1=0xFC;		  //重新赋值 1ms
 TL1=0x66;
 if(tt==5)
 { 
	 tmp=getTmpValue();
	 display(speed,tmp);
	 sendChangeCmd();
	 tt=0; 
 }
 tt++;
}
/*------------------------------------------------
                  外部中断0中断处理
------------------------------------------------*/
void EX0_ISR (void) interrupt 0 //外部中断0服务函数
{
	static unsigned char  i;             //接收红外信号处理
	static bit startflag;                //是否开始处理标志位

	if(startflag)                         
	{
		if(irtime<17&&irtime>=9)//引导码 TC9012的头码，9ms+4.5ms
	    	i=0;
		irdata[i]=irtime;//存储每个电平的持续时间，用于以后判断是0还是1
		irtime=0;
		i++;
		if(i==33)
		{
		 irok=1;
		 i=0;
		}
	}
	else
	{
		irtime=0;
		startflag=1;
	}

}