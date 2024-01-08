/**********************BST-M51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：24L01无线接收实验
*  公司：深圳市亚博软件开发有限公司       
*  日期：2015-7
*  晶振:11.0592MHZ
******************************************************************/
#include"reg52.h"
#include"nrf24l01.h"
sbit CE=P1^0;  //RX/TX模式选择端	
sbit SCLK=P1^1;	//SPI时钟端			   
sbit MISO=P1^2;	//SPI主机输出从机输出端	 
sbit CSN=P1^3; //SPI片选端//就是SS	  
sbit MOSI=P1^4;	//SPI主机输出从机输入端	 
sbit IRQ=P1^5; //可屏蔽中断端			 
sbit wei1=P2^4;
sbit wei2=P2^5;
sbit wei3=P2^6;
sbit wei4=P2^7;
unsigned char Date[5];//最后一位用来存放结束标志
unsigned char code TxAddr[]={0x34,0x43,0x10,0x10,0x01};//发送地址
/*****************状态标志*****************************************/
unsigned char  bdata sta;   //状态标志
sbit RX_DR=sta^6;
sbit TX_DS=sta^5;
sbit MAX_RT=sta^4;

/**********************延时函数**************************/
void NRFDelay(unsigned int t)
{
   unsigned int x,y;
   for(x=t;x>0;x--)
    for(y=110;y>0;y--);
}
/**********************数码管显示*****************************/
unsigned char Shumaguan(unsigned char *x)
{
			wei1=1;
			wei2=0;
			wei3=0;
			wei4=0;
			P0=x[0];
			NRFDelay(2);
			wei1=0;
			wei2=1;
			wei3=0;
			wei4=0;
			P0=x[1];
			NRFDelay(2);
			wei1=0;
			wei2=0;
			wei3=1;
			wei4=0;
			P0=x[2];
			NRFDelay(2);
			wei1=0;
			wei2=0;
			wei3=0;
			wei4=1;
			P0=x[3];
			NRFDelay(2);	   
}
/*****************SPI时序函数******************************************/
unsigned char NRFSPI(unsigned char date)
{
    unsigned char i;
   	for(i=0;i<8;i++)          // 循环8次
   	{
	  if(date&0x80)
	    MOSI=1;
	  else
	    MOSI=0;   // byte最高位输出到MOSI
   	  date<<=1;             // 低一位移位到最高位
   	  SCLK=1; 
	  if(MISO)               // 拉高SCK，nRF24L01从MOSI读入1位数据，同时从MISO输出1位数据
   	    date|=0x01;       	// 读MISO到byte最低位
   	  SCLK=0;            	// SCK置低
   	}
    return(date);           	// 返回读出的一字节
}
/**********************NRF24L01初始化函数*******************************/
void NRF24L01Int()
{
	NRFDelay(2);//让系统什么都不干
	CE=0;
	CSN=1;  
	SCLK=0;
	IRQ=1; 
}
/*****************SPI读寄存器一字节函数*********************************/
unsigned char NRFReadReg(unsigned char RegAddr)
{
   unsigned char BackDate;
   CSN=0;//启动时序
   NRFSPI(RegAddr);//写寄存器地址
   BackDate=NRFSPI(0x00);//写入读寄存器指令  
   CSN=1;
   return(BackDate); //返回状态
}
/*****************SPI写寄存器一字节函数*********************************/
unsigned char NRFWriteReg(unsigned char RegAddr,unsigned char date)
{
   unsigned char BackDate;
   CSN=0;//启动时序
   BackDate=NRFSPI(RegAddr);//写入地址
   NRFSPI(date);//写入值
   CSN=1;  
   return(BackDate);
}
/*****************SPI读取RXFIFO寄存器的值********************************/
unsigned char NRFReadRxDate(unsigned char RegAddr,unsigned char *RxDate,unsigned char DateLen)
{  //寄存器地址//读取数据存放变量//读取数据长度//用于接收
    unsigned char BackDate,i;
	CSN=0;//启动时序
	BackDate=NRFSPI(RegAddr);//写入要读取的寄存器地址
	for(i=0;i<DateLen;i++) //读取数据
	  {
	     RxDate[i]=NRFSPI(0);
	  } 
    CSN=1;
   return(BackDate); 
}
/*****************SPI写入TXFIFO寄存器的值**********************************/
unsigned char NRFWriteTxDate(unsigned char RegAddr,unsigned char *TxDate,unsigned char DateLen)
{ //寄存器地址//写入数据存放变量//读取数据长度//用于发送
   unsigned char BackDate,i;
   CSN=0;
   BackDate=NRFSPI(RegAddr);//写入要写入寄存器的地址
   for(i=0;i<DateLen;i++)//写入数据
     {
	    NRFSPI(*TxDate++);
	 }   
   CSN=1;
   return(BackDate);
}
/*****************NRF设置为发送模式并发送数据******************************/
void NRFSetTxMode(unsigned char *TxDate)
{  //发送模式 
    CE=0;   
   	NRFWriteTxDate(W_REGISTER+TX_ADDR,TxAddr,TX_ADDR_WITDH);//写寄存器指令+P0地址使能指令+发送地址+地址宽度
	NRFWriteTxDate(W_REGISTER+RX_ADDR_P0,TxAddr,TX_ADDR_WITDH);//为了应答接收设备，接收通道0地址和发送地址相同
	NRFWriteTxDate(W_TX_PAYLOAD,TxDate,TX_DATA_WITDH);//写入数据 
	/******下面有关寄存器配置**************/
  	NRFWriteReg(W_REGISTER+EN_AA,0x01);       // 使能接收通道0自动应答
  	NRFWriteReg(W_REGISTER+EN_RXADDR,0x01);   // 使能接收通道0
  	NRFWriteReg(W_REGISTER+SETUP_RETR,0x0a);  // 自动重发延时等待250us+86us，自动重发10次
  	NRFWriteReg(W_REGISTER+RF_CH,0x40);         // 选择射频通道0x40
  	NRFWriteReg(W_REGISTER+RF_SETUP,0x07);    // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
  	NRFWriteReg(W_REGISTER+CONFIG,0x0e);      // CRC使能，16位CRC校验，上电	
	CE=1;	
	NRFDelay(5);//保持10us秒以上
} 
/*****************NRF设置为接收模式并接收数据******************************/
//接收模式
void NRFSetRXMode()
{
    CE=0;
  	NRFWriteTxDate(W_REGISTER+RX_ADDR_P0,TxAddr,TX_ADDR_WITDH);  // 接收设备接收通道0使用和发送设备相同的发送地址
  	NRFWriteReg(W_REGISTER+EN_AA,0x01);               // 使能接收通道0自动应答
  	NRFWriteReg(W_REGISTER+EN_RXADDR,0x01);           // 使能接收通道0
  	NRFWriteReg(W_REGISTER+RF_CH,0x40);                 // 选择射频通道0x40
  	NRFWriteReg(W_REGISTER+RX_PW_P0,TX_DATA_WITDH);  // 接收通道0选择和发送通道相同有效数据宽度
  	NRFWriteReg(W_REGISTER+RF_SETUP,0x07);            // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
	NRFWriteReg(W_REGISTER+CONFIG,0x0f);             // CRC使能，16位CRC校验，上电，接收模式     
  	CE = 1; 
	NRFDelay(5);    
}
/****************************检测是否有接收到数据******************************/
void CheckACK()
{  //用于发射模式接收应答信号
	sta=NRFReadReg(R_REGISTER+STATUS);                    // 返回状态寄存器
	if(TX_DS)
	   NRFWriteReg(W_REGISTER+STATUS,0xff);  // 清除TX_DS或MAX_RT中断标志
}
/*************************接收数据*********************************************/
void GetDate()				 
{
	int i;
    sta=NRFReadReg(R_REGISTER+STATUS);//发送数据后读取状态寄存器
    if(RX_DR)				// 判断是否接收到数据
     {
	   CE=0;//待机
	   NRFReadRxDate(R_RX_PAYLOAD,Date,RX_DATA_WITDH);// 从RXFIFO读取数据 接收4位即可，后一位位结束位
	   NRFWriteReg(W_REGISTER+STATUS,0xff); //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标
	   CSN=0;
	   NRFSPI(FLUSH_RX);//用于清空FIFO ！！关键！！不然会出现意想不到的后果！！！大家记住！！ 
	   CSN=1;		 
     }
    //NRFWriteReg(W_REGISTER+STATUS,0xff); //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标	  
	for(i=0;i<100;i++)
		Shumaguan(Date);
} 