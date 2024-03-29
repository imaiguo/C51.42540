#ifndef _NRF24L01_H_
#define _NRF24L01_H_


/*******************************************************/
#define TX_ADDR_WITDH 5	//发送地址宽度设置为5个字节
#define RX_ADDR_WITDH 5	//接收地址宽度设置为5个字节
#define TX_DATA_WITDH 2//发送数据宽度1个字节
#define RX_DATA_WITDH 2//接收数据宽度1个字节

/*******************命令寄存器***************************/
#define  READ_REG        0x00//读取配置寄存器
#define  WRITE_REG       0x20//写配置寄存器
#define  RD_RX_PLOAD 	 0x61//读取RX有效数据
#define  WR_TX_PLOAD	 0xa0//写TX有效数据
#define  FLUSH_TX		 0xe1//清除TXFIFO寄存器
#define  FLUSH_RX		 0xe2//清除RXFIFO寄存器
#define  REUSE_TX_PL     0xe3//重新使用上一包有效数据
#define  NOP             0xff//空操作
/******************寄存器地址****************************/

#define  CONFIG          0x00//配置寄存器
#define  EN_AA			 0x01//使能自动应答
#define  EN_RXADDR       0x02//接收通道使能0-5个通道
#define  SETUP_AW        0x03//设置数据通道地址宽度3-5
#define  SETUP_RETR      0x04//建立自动重发
#define  RF_CH           0x05//射频通道设置
#define  RF_SETUP        0x06//射频寄存器
#define  STATUS          0x07//状态寄存器
#define  OBSERVE_TX      0x08//发送检测寄存器
#define  CD              0x09//载波
#define  RX_ADDR_P0      0x0a//数据通道0接收地址
#define  RX_ADDR_P1      0x0b//数据通道1接收地址
#define  RX_ADDR_P2      0x0c//数据通道2接收地址
#define  RX_ADDR_P3      0x0d//数据通道3接收地址
#define  RX_ADDR_P4      0x0e//数据通道4接收地址
#define  RX_ADDR_P5      0x0f//数据通道5接收地址
#define  TX_ADDR         0x10//发送地址
#define  RX_PW_P0        0x11//P0通道数据宽度设置
#define  RX_PW_P1        0x12//P1通道数据宽度设置
#define  RX_PW_P2        0x13//P2通道数据宽度设置
#define  RX_PW_P3        0x14//P3通道数据宽度设置
#define  RX_PW_P4        0x15//P4通道数据宽度设置
#define  RX_PW_P5        0x16//P5通道数据宽度设置
#define  FIFO_STATUS     0x17//FIFO状态寄存器


/*******************相关函数声明**************************/
 
unsigned char NRFSPI(unsigned char date);
unsigned char NRFReadReg(unsigned char RegAddr);
unsigned char NRFWriteReg(unsigned char RegAddr,unsigned char date);
unsigned char NRFRead_Buf(unsigned char RegAddr,unsigned char *RxDate,unsigned char DateLen);
unsigned char NRFWrite_Buf(unsigned char RegAddr,unsigned char *TxDate,unsigned char DateLen);


extern void NRF24L01Int();
 
extern void NRFSetRXMode();
extern void GetDate();

extern void NRFSetTxMode(unsigned char *TxDate);
extern unsigned char CheckACK();
 

extern unsigned char bdata sta;
extern idata unsigned char RxData_Buf[RX_DATA_WITDH];			   //保存接收到的数据  
/*********************************************************/
#endif