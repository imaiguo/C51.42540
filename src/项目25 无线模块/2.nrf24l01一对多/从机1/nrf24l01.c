/**********************BST-M51ʵ�鿪��������************************
*  ƽ̨��BST-M51 + Keil U4 + STC89C52
*  ���ƣ�24L01���߽��գ��ӻ�1��
*  ��˾���������ǲ�����������޹�˾       
*  ���ڣ�2015-6
*  ����:11.0592MHZ
******************************************************************/
#include<reg52.h>
#include"NRF24L01.h"
#include"Delay.h"
#include"uart.h"


sbit CE=P1^0;  //RX/TXģʽѡ���	
sbit SCLK=P1^1;	//SPIʱ�Ӷ�			   
sbit MISO=P1^2;	//SPI��������ӻ������	 
sbit CSN=P1^3; //SPIƬѡ��//����SS	  
sbit MOSI=P1^4;	//SPI��������ӻ������	 
sbit IRQ=P1^5; //�������ж϶�			 
sbit wei1=P2^4;
sbit wei2=P2^5;
sbit wei3=P2^6;
sbit wei4=P2^7;

sbit buzzer = P2^3 ;

unsigned char RxData_Buf0[RX_DATA_WITDH];    			//�洢ͨ��0���յ������ݣ��������͹�����

unsigned char  code Rx_Addr1[RX_ADDR_WITDH]={0xc1,0xc2,0xc2,0xc2,0xc2}; 
 
unsigned char  code Tx_Addr1[TX_ADDR_WITDH]={0xc1,0xc2,0xc2,0xc2,0xc2};//���͵�ַ


/*****************״̬��־*****************************************/
unsigned char bdata sta;   //״̬��־
sbit RX_DR=sta^6;
sbit TX_DS=sta^5;
sbit MAX_RT=sta^4;


/**********************NRF24L01��ʼ������*******************************/
void NRF24L01Int()
{
 
	CE  = 0;        // ����
	CSN = 1;        // SPI��ֹ
	SCLK= 0;        // SPIʱ���õ�
	IRQ = 1;        // �жϸ�λ
}

/**********************�������ʾ*****************************/
unsigned char Shumaguan(unsigned char x)
{
			wei1=1;
			wei2=0;
			wei3=0;
			wei4=0;
			P0=x;	   
}

/**************************************************
������NRFSPI(uchar date)

������
    ����SPIЭ�飬дһ�ֽ����ݵ�nRF24L01��ͬʱ��nRF24L01
	����һ�ֽ�
/**************************************************/
unsigned char NRFSPI(unsigned char date)
{
    unsigned char i;
   	for(i=0;i<8;i++)          // ѭ��8��
   	{
   	  MOSI = (date & 0x80);    // byte���λ�����MOSI
   	  date<<=1;                // ��һλ��λ�����λ
   	  SCLK=1; 			       // ����SCK��nRF24L01��MOSI����1λ���ݣ�ͬʱ��MISO���1λ����
   	  date |= MISO;        	   // ��MISO��byte���λ
   	  SCLK=0;                  // SCK�õ�
   	}
    return(date);           	// ���ض�����һ�ֽ�
}


/**************************************************
������NRFReadReg(uchar RegAddr)

������
    ��reg�Ĵ�����һ�ֽ�
/**************************************************/
unsigned char NRFReadReg(unsigned char RegAddr)
{
   unsigned char BackDate;
   CSN=0;                          //CSN�õͣ���ʼ��������
   NRFSPI(RegAddr);                //д�Ĵ�����ַ, ѡ��Ĵ���
   BackDate=NRFSPI(0x00);          //Ȼ��ӸüĴ���������  
   CSN=1;						   // CSN���ߣ��������ݴ���
   return(BackDate);               // ���ؼĴ�������
}



/**************************************************
������NRFWriteReg(uchar RegAddr,uchar date)

������
    д����value��reg�Ĵ���
/**************************************************/
unsigned char NRFWriteReg(unsigned char RegAddr,unsigned char date)
{
   unsigned char BackDate;
   CSN=0;                        // CSN�õͣ���ʼ��������
   BackDate=NRFSPI(RegAddr);     // ѡ��Ĵ�����ͬʱ����״̬��
   NRFSPI(date);                 // Ȼ��д���ݵ��üĴ���
   CSN=1;
   return(BackDate);
}




/**************************************************
������NRFRead_Buf(uchar RegAddr,uchar *RxDate,uchar DateLen)
		    //�Ĵ�����ַ//��ȡ���ݴ�ű���//��ȡ���ݳ���
RX			//���ڽ���
������
    ��reg�Ĵ�������bytes���ֽڣ�ͨ��������ȡ����ͨ��
	���ݻ����/���͵�ַ
/**************************************************/
unsigned char NRFRead_Buf(unsigned char RegAddr,unsigned char *RxDate,unsigned char DateLen)
{ 
    unsigned char BackDate,i;
	CSN=0;                           //����ʱ��
	BackDate=NRFSPI(RegAddr);        //д��Ҫ��ȡ�ļĴ�����ַ��ͬʱ����״̬��
	for(i=0;i<DateLen;i++)           //��ȡ����
	  {
	     RxDate[i]=NRFSPI(0);		 // ����ֽڴ�nRF24L01����
	  } 
    CSN=1;
   return(BackDate); 
}




/**************************************************
������NRFWrite_Buf(uchar RegAddr,uchar *TxDate,uchar DateLen)
		    //�Ĵ�����ַ//д�����ݴ�ű���//��ȡ���ݳ���
TX			//���ڷ���
������
    ��pBuf�����е�����д�뵽nRF24L01��ͨ������д�뷢
	��ͨ�����ݻ����/���͵�ַ
/**************************************************/
unsigned char NRFWrite_Buf(unsigned char RegAddr,unsigned char *TxDate,unsigned char DateLen)
{
   unsigned char BackDate,i;
   CSN=0;
   BackDate=NRFSPI(RegAddr);     // ѡ��Ĵ�����ͬʱ����״̬��
   for(i=0;i<DateLen;i++)        //д������
     {
	    NRFSPI(*TxDate++);
	 }   
   CSN=1;
   return(BackDate);
}






/********************************************************* 
*														 *
*														 *
************NRF����Ϊ����ģʽ����������*******************
*														 *
*														 *
*********************************************************/

void NRFSetRXMode()													   //��Ҫ����ģʽ
{
	CE=0;  	


  	NRFWrite_Buf(WRITE_REG + RX_ADDR_P1 , Tx_Addr1 , TX_ADDR_WITDH);  // �����豸����ͨ��0 �� �����豸�ķ��͵�ַ��ͬ	     
  	NRFWriteReg(WRITE_REG + RX_PW_P1 , TX_DATA_WITDH);     // ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
	NRFWriteReg(WRITE_REG + CONFIG, 0x0f);                 // CRCʹ�ܣ�16λCRCУ�飬�ϵ�  ����ģʽ
  	NRFWriteReg(WRITE_REG + EN_RXADDR, 0x02);              // ʹ�ܽ���ͨ��0   
  	NRFWriteReg(WRITE_REG + EN_AA, 0x02);                  // ʹ�ܽ���ͨ��0�Զ�Ӧ��
	NRFWriteReg(WRITE_REG + RF_CH, 0x40);                  // ѡ����Ƶͨ��0x40  
	NRFWriteReg(WRITE_REG + RF_SETUP, 0x07);               // ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������
 
  	CE = 1;
	Delay(5);//����10us������     
}
 
void GetDate()				 		   //  �������� 
{
    sta=NRFReadReg(READ_REG+STATUS);  //�������ݺ��ȡ״̬�Ĵ���
    if(RX_DR)				          // �ж��Ƿ���յ����ݣ��ӵ��ʹ�RXȡ��
     {
 
	   CE=0;                         //����
 
 	   NRFRead_Buf(RD_RX_PLOAD,RxData_Buf0,RX_DATA_WITDH) ;   // ��RXFIFO��ȡ������������

	   buzzer=0;
	   Delay(100);
	   buzzer=1;

	   uart(*RxData_Buf0);		
 
	   NRFWriteReg(WRITE_REG+STATUS,0xff);              //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�־
	   
	   CSN=0;
	   NRFSPI(FLUSH_RX);                                //�������FIFO �����ؼ�������Ȼ��������벻���ĺ����������Ҽ�ס���� 
	   CSN=1;		 
     
     }
	  Shumaguan(*RxData_Buf0);
} 


/********************************************************* 
*														 *
*														 *
************NRF����Ϊ����ģʽ����������*******************
*														 *
*														 *
*********************************************************/

void NRFSetTxMode(unsigned char *TxDate)							  //����ģʽ
{
    CE=0; 
   	NRFWrite_Buf(WRITE_REG + TX_ADDR , Rx_Addr1, TX_ADDR_WITDH);    //д�Ĵ���ָ��+���յ�ַʹ��ָ����յ�ַ����ַ���
	NRFWrite_Buf(WRITE_REG + RX_ADDR_P1 , Rx_Addr1 , TX_ADDR_WITDH); //Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
	NRFWrite_Buf(WR_TX_PLOAD , TxDate , TX_DATA_WITDH);              //д������ 

 
	/******�����йؼĴ�������**************/
  	NRFWriteReg(WRITE_REG+EN_AA,0x02);       // ʹ�ܽ���ͨ��0�Զ�Ӧ��
  	NRFWriteReg(WRITE_REG+EN_RXADDR,0x02);   // ʹ�ܽ���ͨ��0
  	NRFWriteReg(WRITE_REG+SETUP_RETR,0x0f);  // �Զ��ط���ʱ�ȴ�****us���Զ��ط�15��, 
	NRFWriteReg(WRITE_REG+RF_CH,0x40);       // ѡ����Ƶͨ��0x40
  	NRFWriteReg(WRITE_REG+RF_SETUP,0x07);    // ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������
	NRFWriteReg(WRITE_REG+CONFIG,0x0f);      // CRCʹ�ܣ�16λCRCУ�飬�ϵ�  
	CE=1;
	Delay(5);//����10us������
}
 
 
unsigned char CheckACK()							         //���Ӧ���źţ����ڷ���ģʽ����Ӧ���ź� 
{ 	
   
	sta = NRFReadReg(READ_REG+STATUS);                    // ����״̬�Ĵ���
	if(TX_DS||MAX_RT)                                     //��������ж�
	{

	   NRFWriteReg(WRITE_REG+STATUS,0xff);                // ���TX_DS��MAX_RT�жϱ�־

	   CSN=0;
	   NRFSPI(FLUSH_TX);      //�������FIFO �����ؼ�������Ȼ��������벻���ĺ����������Ҽ�ס����  
       CSN=1; 

	   return(0);
	}
	else
	   return(1);
} 