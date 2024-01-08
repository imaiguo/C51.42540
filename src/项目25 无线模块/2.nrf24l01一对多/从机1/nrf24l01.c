/**********************BST-M51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：24L01无线接收（从机1）
*  公司：深圳市亚博软件开发有限公司       
*  日期：2015-6
*  晶振:11.0592MHZ
******************************************************************/
#include<reg52.h>
#include"NRF24L01.h"
#include"Delay.h"
#include"uart.h"


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

sbit buzzer = P2^3 ;

unsigned char RxData_Buf0[RX_DATA_WITDH];    			//存储通道0接收到的数据，主机发送过来的

unsigned char  code Rx_Addr1[RX_ADDR_WITDH]={0xc1,0xc2,0xc2,0xc2,0xc2}; 
 
unsigned char  code Tx_Addr1[TX_ADDR_WITDH]={0xc1,0xc2,0xc2,0xc2,0xc2};//发送地址


/*****************状态标志*****************************************/
unsigned char bdata sta;   //状态标志
sbit RX_DR=sta^6;
sbit TX_DS=sta^5;
sbit MAX_RT=sta^4;


/**********************NRF24L01初始化函数*******************************/
void NRF24L01Int()
{
 
	CE  = 0;        // 待机
	CSN = 1;        // SPI禁止
	SCLK= 0;        // SPI时钟置低
	IRQ = 1;        // 中断复位
}

/**********************数码管显示*****************************/
unsigned char Shumaguan(unsigned char x)
{
			wei1=1;
			wei2=0;
			wei3=0;
			wei4=0;
			P0=x;	   
}

/**************************************************
函数：NRFSPI(uchar date)

描述：
    根据SPI协议，写一字节数据到nRF24L01，同时从nRF24L01
	读出一字节
/**************************************************/
unsigned char NRFSPI(unsigned char date)
{
    unsigned char i;
   	for(i=0;i<8;i++)          // 循环8次
   	{
   	  MOSI = (date & 0x80);    // byte最高位输出到MOSI
   	  date<<=1;                // 低一位移位到最高位
   	  SCLK=1; 			       // 拉高SCK，nRF24L01从MOSI读入1位数据，同时从MISO输出1位数据
   	  date |= MISO;        	   // 读MISO到byte最低位
   	  SCLK=0;                  // SCK置低
   	}
    return(date);           	// 返回读出的一字节
}


/**************************************************
函数：NRFReadReg(uchar RegAddr)

描述：
    从reg寄存器读一字节
/**************************************************/
unsigned char NRFReadReg(unsigned char RegAddr)
{
   unsigned char BackDate;
   CSN=0;                          //CSN置低，开始传输数据
   NRFSPI(RegAddr);                //写寄存器地址, 选择寄存器
   BackDate=NRFSPI(0x00);          //然后从该寄存器读数据  
   CSN=1;						   // CSN拉高，结束数据传输
   return(BackDate);               // 返回寄存器数据
}



/**************************************************
函数：NRFWriteReg(uchar RegAddr,uchar date)

描述：
    写数据value到reg寄存器
/**************************************************/
unsigned char NRFWriteReg(unsigned char RegAddr,unsigned char date)
{
   unsigned char BackDate;
   CSN=0;                        // CSN置低，开始传输数据
   BackDate=NRFSPI(RegAddr);     // 选择寄存器，同时返回状态字
   NRFSPI(date);                 // 然后写数据到该寄存器
   CSN=1;
   return(BackDate);
}




/**************************************************
函数：NRFRead_Buf(uchar RegAddr,uchar *RxDate,uchar DateLen)
		    //寄存器地址//读取数据存放变量//读取数据长度
RX			//用于接收
描述：
    从reg寄存器读出bytes个字节，通常用来读取接收通道
	数据或接收/发送地址
/**************************************************/
unsigned char NRFRead_Buf(unsigned char RegAddr,unsigned char *RxDate,unsigned char DateLen)
{ 
    unsigned char BackDate,i;
	CSN=0;                           //启动时序
	BackDate=NRFSPI(RegAddr);        //写入要读取的寄存器地址，同时返回状态字
	for(i=0;i<DateLen;i++)           //读取数据
	  {
	     RxDate[i]=NRFSPI(0);		 // 逐个字节从nRF24L01读出
	  } 
    CSN=1;
   return(BackDate); 
}




/**************************************************
函数：NRFWrite_Buf(uchar RegAddr,uchar *TxDate,uchar DateLen)
		    //寄存器地址//写入数据存放变量//读取数据长度
TX			//用于发送
描述：
    把pBuf缓存中的数据写入到nRF24L01，通常用来写入发
	射通道数据或接收/发送地址
/**************************************************/
unsigned char NRFWrite_Buf(unsigned char RegAddr,unsigned char *TxDate,unsigned char DateLen)
{
   unsigned char BackDate,i;
   CSN=0;
   BackDate=NRFSPI(RegAddr);     // 选择寄存器，同时返回状态字
   for(i=0;i<DateLen;i++)        //写入数据
     {
	    NRFSPI(*TxDate++);
	 }   
   CSN=1;
   return(BackDate);
}






/********************************************************* 
*														 *
*														 *
************NRF设置为接收模式并接收数据*******************
*														 *
*														 *
*********************************************************/

void NRFSetRXMode()													   //主要接收模式
{
	CE=0;  	


  	NRFWrite_Buf(WRITE_REG + RX_ADDR_P1 , Tx_Addr1 , TX_ADDR_WITDH);  // 接收设备接收通道0 和 发送设备的发送地址相同	     
  	NRFWriteReg(WRITE_REG + RX_PW_P1 , TX_DATA_WITDH);     // 接收通道0选择和发送通道相同有效数据宽度
	NRFWriteReg(WRITE_REG + CONFIG, 0x0f);                 // CRC使能，16位CRC校验，上电  接收模式
  	NRFWriteReg(WRITE_REG + EN_RXADDR, 0x02);              // 使能接收通道0   
  	NRFWriteReg(WRITE_REG + EN_AA, 0x02);                  // 使能接收通道0自动应答
	NRFWriteReg(WRITE_REG + RF_CH, 0x40);                  // 选择射频通道0x40  
	NRFWriteReg(WRITE_REG + RF_SETUP, 0x07);               // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
 
  	CE = 1;
	Delay(5);//保持10us秒以上     
}
 
void GetDate()				 		   //  接收数据 
{
    sta=NRFReadReg(READ_REG+STATUS);  //发送数据后读取状态寄存器
    if(RX_DR)				          // 判断是否接收到数据，接到就从RX取出
     {
 
	   CE=0;                         //待机
 
 	   NRFRead_Buf(RD_RX_PLOAD,RxData_Buf0,RX_DATA_WITDH) ;   // 从RXFIFO读取数据主机数据

	   buzzer=0;
	   Delay(100);
	   buzzer=1;

	   uart(*RxData_Buf0);		
 
	   NRFWriteReg(WRITE_REG+STATUS,0xff);              //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志
	   
	   CSN=0;
	   NRFSPI(FLUSH_RX);                                //用于清空FIFO ！！关键！！不然会出现意想不到的后果！！！大家记住！！ 
	   CSN=1;		 
     
     }
	  Shumaguan(*RxData_Buf0);
} 


/********************************************************* 
*														 *
*														 *
************NRF设置为发送模式并发送数据*******************
*														 *
*														 *
*********************************************************/

void NRFSetTxMode(unsigned char *TxDate)							  //发送模式
{
    CE=0; 
   	NRFWrite_Buf(WRITE_REG + TX_ADDR , Rx_Addr1, TX_ADDR_WITDH);    //写寄存器指令+接收地址使能指令，接收地址，地址宽度
	NRFWrite_Buf(WRITE_REG + RX_ADDR_P1 , Rx_Addr1 , TX_ADDR_WITDH); //为了应答接收设备，接收通道0地址和发送地址相同
	NRFWrite_Buf(WR_TX_PLOAD , TxDate , TX_DATA_WITDH);              //写入数据 

 
	/******下面有关寄存器配置**************/
  	NRFWriteReg(WRITE_REG+EN_AA,0x02);       // 使能接收通道0自动应答
  	NRFWriteReg(WRITE_REG+EN_RXADDR,0x02);   // 使能接收通道0
  	NRFWriteReg(WRITE_REG+SETUP_RETR,0x0f);  // 自动重发延时等待****us，自动重发15次, 
	NRFWriteReg(WRITE_REG+RF_CH,0x40);       // 选择射频通道0x40
  	NRFWriteReg(WRITE_REG+RF_SETUP,0x07);    // 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益
	NRFWriteReg(WRITE_REG+CONFIG,0x0f);      // CRC使能，16位CRC校验，上电  
	CE=1;
	Delay(5);//保持10us秒以上
}
 
 
unsigned char CheckACK()							         //检测应答信号，用于发射模式接收应答信号 
{ 	
   
	sta = NRFReadReg(READ_REG+STATUS);                    // 返回状态寄存器
	if(TX_DS||MAX_RT)                                     //发送完毕中断
	{

	   NRFWriteReg(WRITE_REG+STATUS,0xff);                // 清除TX_DS或MAX_RT中断标志

	   CSN=0;
	   NRFSPI(FLUSH_TX);      //用于清空FIFO ！！关键！！不然会出现意想不到的后果！！！大家记住！！  
       CSN=1; 

	   return(0);
	}
	else
	   return(1);
} 