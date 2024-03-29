/**********************BST-M51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：火焰传感器报警实验
*  公司：深圳市亚博软件开发有限公司       
*  日期：2015-6
*  晶振:11.0592MHZ
******************************************************************/
#include<reg52.h>    //包含单片机寄存器的头文件
#include <intrins.h> 

#define  AddWr 0x90    //PCF8591 地址

// 变量定义
unsigned char AD_CHANNEL;
unsigned char  D[32];

sbit scl=P2^0;       //I2C  时钟 
sbit sda=P2^1;       //I2C  数据
sbit beep=P2^3;      //蜂鸣器 
bit ack;                 /*应答标志位*/

/*******************************************************************
                     起动总线函数               
函数原型: void  Start_I2c();  
功能:     启动I2C总线,即发送I2C起始条件.  
********************************************************************/
void Start_I2c()
{
  sda=1;         /*发送起始条件的数据信号*/
  _nop_();
  scl=1;
  _nop_();        /*起始条件建立时间大于4.7us,延时*/
  _nop_();
  _nop_();
  _nop_();
  _nop_();    
  sda=0;         /*发送起始信号*/
  _nop_();        /* 起始条件锁定时间大于4μs*/
  _nop_();
  _nop_();
  _nop_();
  _nop_();       
  scl=0;       /*钳住I2C总线，准备发送或接收数据 */
  _nop_();
  _nop_();
}

/*******************************************************************
                      结束总线函数               
函数原型: void  Stop_I2c();  
功能:     结束I2C总线,即发送I2C结束条件.  
********************************************************************/
void Stop_I2c()
{
  sda=0;      /*发送结束条件的数据信号*/
  _nop_();       /*发送结束条件的时钟信号*/
  scl=1;      /*结束条件建立时间大于4μs*/
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  sda=1;      /*发送I2C总线结束信号*/
  _nop_();
  _nop_();
  _nop_();
  _nop_();
}

/*******************************************************************
                 字节数据发送函数               
函数原型: void  I2C_SendByte(UCHAR c);
功能:     将数据c发送出去,可以是地址,也可以是数据,发完后等待应答,并对
          此状态位进行操作.(不应答或非应答都使ack=0)     
           发送数据正常，ack=1; ack=0表示被控器无应答或损坏。
********************************************************************/
void  I2C_SendByte(unsigned char  c)
{
 unsigned char  i;
 
 for(i=0;i<8;i++)  /*要传送的数据长度为8位*/
    {
     if((c<<i)&0x80)sda=1;   /*判断发送位*/
       else  sda=0;                
     _nop_();
     scl=1;               /*置时钟线为高，通知被控器开始接收数据位*/
      _nop_(); 
      _nop_();             /*保证时钟高电平周期大于4μs*/
      _nop_();
      _nop_();
      _nop_();         
     scl=0; 
    }
    
    _nop_();
    _nop_();
    sda=1;                /*8位发送完后释放数据线，准备接收应答位*/
    _nop_();
    _nop_();   
    scl=1;
    _nop_();
    _nop_();
    _nop_();
    if(sda==1)ack=0;     
       else ack=1;        /*判断是否接收到应答信号*/
    scl=0;
    _nop_();
    _nop_();
}

/*******************************************************************
                 字节数据接收函数               
函数原型: UCHAR  I2C_RcvByte();
功能:        用来接收从器件传来的数据,并判断总线错误(不发应答信号)，
          发完后请用应答函数应答从机。  
********************************************************************/    
unsigned char   I2C_RcvByte()
{
  unsigned char  retc=0,i; 
  sda=1;                     /*置数据线为输入方式*/
  for(i=0;i<8;i++)
      {
        _nop_();           
        scl=0;                  /*置时钟线为低，准备接收数据位*/
        _nop_();
        _nop_();                 /*时钟低电平周期大于4.7μs*/
        _nop_();
        _nop_();
        _nop_();
        scl=1;                  /*置时钟线为高使数据线上数据有效*/
        _nop_();
        _nop_();
        retc=retc<<1;
        if(sda==1)retc=retc+1;  /*读数据位,接收的数据位放入retc中 */
        _nop_();
        _nop_(); 
      }
  scl=0;    
  _nop_();
  _nop_();
  return(retc);
}

/********************************************************************
                     应答子函数
函数原型:  void Ack_I2c(bit a);
功能:      主控器进行应答信号(可以是应答或非应答信号，由位参数a决定)
********************************************************************/
void Ack_I2c(bit a)
{  
  if(a==0)sda=0;              /*在此发出应答或非应答信号 */
  else sda=1;				  /*0为发出应答，1为非应答信号 */
  _nop_();
  _nop_();
  _nop_();      
  scl=1;
  _nop_();
  _nop_();                    /*时钟低电平周期大于4μs*/
  _nop_();
  _nop_();
  _nop_();  
  scl=0;                     /*清时钟线，住I2C总线以便继续接收*/
  _nop_();
  _nop_();    
}

/************************************************************
* 函数名        : Pcf8591_DaConversion
* 函数功能      : PCF8591的输出端输出模拟量
* 输入          : addr（器件地址），channel（转换通道），value（转换的数值）
* 输出         	: 无
******************* *****************************************/
bit Pcf8591_DaConversion(unsigned char addr,unsigned char channel,  unsigned char Val)
{
   Start_I2c();              //启动总线
   I2C_SendByte(addr);            //发送器件地址
   if(ack==0)return(0);
   I2C_SendByte(0x40|channel);              //发送控制字节
   if(ack==0)return(0);
   I2C_SendByte(Val);            //发送DAC的数值  
   if(ack==0)return(0);
   Stop_I2c();               //结束总线
   return(1);
}

/************************************************************
* 函数名        : Pcf8591_SendByte
* 函数功能		: 写入一个控制命令
* 输入          : addr（器件地址），channel（转换通道）
* 输出         	: 无
************************************************************/
bit PCF8591_SendByte(unsigned char addr,unsigned char channel)
{
   Start_I2c();              //启动总线
   I2C_SendByte(addr);            //发送器件地址
   if(ack==0)return(0);
   I2C_SendByte(0x40|channel);              //发送控制字节
   if(ack==0)return(0);
   Stop_I2c();               //结束总线
   return(1);
}

/************************************************************
* 函数名       	: PCF8591_RcvByte
* 函数功能   	: 读取一个转换值
* 输入          :
* 输出          : dat
************************************************************/
unsigned char PCF8591_RcvByte(unsigned char addr)
{  
   unsigned char dat;

   Start_I2c();          //启动总线
   I2C_SendByte(addr+1);      //发送器件地址
   if(ack==0)return(0);
   dat=I2C_RcvByte();          //读取数据0

   Ack_I2c(1);           //发送非应答信号
   Stop_I2c();           //结束总线
   return(dat);
}
void delay()		   //大概0.5ms左右的延时
{
	unsigned char a;
	for(a=450;a>0;a--)
	{
	}
}
/*------------------------------------------------
                    主函数
------------------------------------------------*/
main()
{  
	while(1)
	{
		PCF8591_SendByte(AddWr,2);
		D[2]=PCF8591_RcvByte(AddWr);   
		if(D[2]>245)
		{
			unsigned int m;
			for(m=800;m>0;m--)	  //持续时间0.5ms*800
			{
				beep=~beep;
	
				delay();		 //2000HZ的信号。
			}
			P1=~P1;
			for(m=500;m>0;m--)	 //持续时间0.5ms*2*500
			{
				beep=~beep;	
				delay();
				delay();		//1000HZ的信号。
			}
			P1=~P1;	
		} 
	}
}