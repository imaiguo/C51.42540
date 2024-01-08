/**********************BST-M51ʵ�鿪��������************************
*  ƽ̨��BST-M51 + Keil U4 + STC89C52
*  ���ƣ�24L01���߷��䣨һ�Զࣩ
*  ��˾���������ǲ�����������޹�˾       
*  ���ڣ�2015-6
*  ����:11.0592MHZ
******************************************************************/
#include<reg52.h>
#include"NRF24L01.h"
#include"Delay.h"
 
sbit CE=P1^0;  //RX/TXģʽѡ���	3
sbit SCLK=P1^1;	//SPIʱ�Ӷ�			   5
sbit MISO=P1^2;	//SPI��������ӻ������	 7
sbit CSN=P1^3; //SPIƬѡ��//����SS	  4
sbit MOSI=P1^4;	//SPI��������ӻ������	 6
sbit IRQ=P1^5; //�������ж϶�			 8

idata unsigned char  RxData_Buf0[RX_DATA_WITDH];    			//�洢ͨ��0���յ�������
idata unsigned char  RxData_Buf1[RX_DATA_WITDH]; 			//�洢ͨ��1���յ�������
//idata uchar RxData_Buf2[RX_DATA_WITDH];    			//�洢ͨ��2���յ�������
//idata uchar RxData_Buf3[RX_DATA_WITDH]; 			//�洢ͨ��3���յ�������
//idata uchar RxData_Buf4[RX_DATA_WITDH];    		//�洢ͨ��4���յ�������
//idata uchar RxData_Buf5[RX_DATA_WITDH]; 			//�洢ͨ��5���յ�������

unsigned char  code Rx_Addr0[RX_ADDR_WITDH]={0xc0,0x43,0x10,0x10,0x01};   //����ͨ��0�Ľ��յ�ַ��ӻ�0�ķ��͵�ַ��ͬ (����ʱ�������͵�ַ���������ݸ��ӻ�0)
unsigned char  code Rx_Addr1[RX_ADDR_WITDH]={0xc1,0xc2,0xc2,0xc2,0xc2};   
unsigned char  code Rx_Addr2[RX_ADDR_WITDH]={0xc2,0xc2,0xc2,0xc2,0xc2};
unsigned char  code Rx_Addr3[RX_ADDR_WITDH]={0xc3,0xc2,0xc2,0xc2,0xc3};
//uchar code Rx_Addr4[1] = {0xc4};
//uchar code Rx_Addr5[1] = {0xc5};

unsigned char code Tx_Addr0[TX_ADDR_WITDH]={0xc0,0x43,0x10,0x10,0x01};//���͵�ַ
unsigned char code Tx_Addr1[TX_ADDR_WITDH]={0xc1,0xc2,0xc2,0xc2,0xc2};//���͵�ַ
unsigned char code Tx_Addr2[TX_ADDR_WITDH]={0xc2,0xc2,0xc2,0xc2,0xc2};//���͵�ַ
unsigned char code Tx_Addr3[TX_ADDR_WITDH]={0xc3,0xc2,0xc2,0xc2,0xc2};//���͵�ַ


 
/*****************״̬��־*****************************************/
unsigned char bdata sta;     //״̬��־
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

void NRFSetRXMode()											  //��Ҫ����ģʽ
{
	CE=0;  	
 
  	NRFWrite_Buf(WRITE_REG + RX_ADDR_P0 , Rx_Addr0 , RX_ADDR_WITDH);  // �����豸����ͨ��0 �� �����豸�ķ��͵�ַ��ͬ	    
  	NRFWrite_Buf(WRITE_REG + RX_ADDR_P1 , Rx_Addr1 , RX_ADDR_WITDH);  // �����豸����ͨ��1 �� �����豸�ķ��͵�ַ��ͬ

  	NRFWriteReg(WRITE_REG + RX_PW_P0 , RX_DATA_WITDH);     // ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
  	NRFWriteReg(WRITE_REG + RX_PW_P1 , RX_DATA_WITDH);     // ����ͨ��1ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
 
	NRFWriteReg(WRITE_REG + CONFIG, 0x0f);                 // CRCʹ�ܣ�16λCRCУ�飬�ϵ�  ����ģʽ
  	NRFWriteReg(WRITE_REG + EN_RXADDR, 0x03);              // ʹ�ܽ���ͨ��0,1   
  	NRFWriteReg(WRITE_REG + EN_AA, 0x03);                  // ʹ�ܽ���ͨ��0,1�Զ�Ӧ��
	NRFWriteReg(WRITE_REG + RF_CH, 0x40);                  // ѡ����Ƶͨ��0x40  
	NRFWriteReg(WRITE_REG + RF_SETUP, 0x07);               // ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������
 
  	CE = 1;
	Delay(5);//����10us������     
}

 
void GetDate()				 		 //  �������ݺ���
{
    unsigned char RX_P_NO;                   //����ͨ����
	sta=NRFReadReg(READ_REG+STATUS); //�������ݺ��ȡ״̬�Ĵ���

    if(RX_DR)			         	 // �ж��Ƿ���յ����ݣ��ӵ��ʹ�RXȡ��
     {
	   RX_P_NO=sta&0x0e;             //��ȡͨ����!!!!!!!!!!!!!!!
	   CE=0;                         //����
	   switch(RX_P_NO)
	    {
		   case 0x00:NRFRead_Buf(RD_RX_PLOAD,RxData_Buf0,RX_DATA_WITDH);break;// ��RXFIFO��ȡ����ͨ��0�����浽RxData_Buf0[]
		   case 0x02:NRFRead_Buf(RD_RX_PLOAD,RxData_Buf1,RX_DATA_WITDH);break;// ��RXFIFO��ȡ����ͨ��1�����浽RxData_Buf1[]
		   default:break;
		}
//	   LcdWriteChStr(1,75,RevTempDate0);//LCD12864Һ����ʾ
//	   LcdWriteChStr(3,75,RevTempDate1);
//     MAX232SendDate();//�������ݵ���λ��	
 
	   NRFWriteReg(WRITE_REG+STATUS,0xff);              //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�־
	   
	   CSN=0;
	   NRFSPI(FLUSH_RX);                                //�������FIFO �����ؼ�������Ȼ��������벻���ĺ����������Ҽ�ס���� 
	   CSN=1;		 
     }
    
} 





/********************************************************* 
*														 *
*														 *
************NRF����Ϊ����ģʽ����������*******************
*														 *
*														 *
*********************************************************/
 
void Tx_Mode()									 //����ģʽ
{
     CE=0; 
	/******�����йؼĴ�������**************/
	NRFWriteReg(WRITE_REG + CONFIG,0x0e);      // CRCʹ�ܣ�16λCRCУ�飬�ϵ�   ����ģʽ
  	NRFWriteReg(WRITE_REG + EN_RXADDR,0x03);   // ʹ�ܽ���ͨ��0��1
  	NRFWriteReg(WRITE_REG + EN_AA,0x03);       // ʹ�ܽ���ͨ��0��1�Զ�Ӧ��
  	NRFWriteReg(WRITE_REG + SETUP_RETR,0x0f);  // �Զ��ط���ʱ�ȴ�****us���Զ��ط�15��, 
	NRFWriteReg(WRITE_REG + RF_CH,0x40);       // ѡ����Ƶͨ��0x40
  	NRFWriteReg(WRITE_REG + RF_SETUP,0x07);    // ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������
	CE=1;
	Delay(5);//����10us������
}


void NRFSetTxMode0(unsigned char *TxDate)				   //���͸��ӻ�0
{
    CE=0;
		 
   	NRFWrite_Buf(WRITE_REG + TX_ADDR , Tx_Addr0 , TX_ADDR_WITDH);       //д�Ĵ���ָ��+���յ�ַʹ��ָ�� �����յ�ַ ����ַ���
	NRFWrite_Buf(WRITE_REG + RX_ADDR_P0 , Tx_Addr0 , TX_ADDR_WITDH);    //Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
	NRFWrite_Buf(WR_TX_PLOAD , TxDate , TX_DATA_WITDH);                 //д������ 
 
	CE=1;
	Delay(5);//����10us������
}

void NRFSetTxMode1(unsigned char *TxDate)				  //���͸��ӻ�1
{
    CE=0;
		 
   	NRFWrite_Buf(WRITE_REG + TX_ADDR , Tx_Addr1 , TX_ADDR_WITDH);       //д�Ĵ���ָ��+���յ�ַʹ��ָ�� �����յ�ַ ����ַ���
	NRFWrite_Buf(WRITE_REG + RX_ADDR_P1 , Tx_Addr1 , TX_ADDR_WITDH);    //Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
	NRFWrite_Buf(WR_TX_PLOAD , TxDate , TX_DATA_WITDH);                 //д������ 
 
	CE=1;
	Delay(5);//����10us������
}

void NRFSetTxMode2(unsigned char *TxDate)				   //���͸��ӻ�2
{
    CE=0;
		 
   	NRFWrite_Buf(WRITE_REG + TX_ADDR , Tx_Addr2 , TX_ADDR_WITDH);       //д�Ĵ���ָ��+���յ�ַʹ��ָ�� �����յ�ַ ����ַ���
	NRFWrite_Buf(WRITE_REG + RX_ADDR_P2 , Tx_Addr2 , TX_ADDR_WITDH);    //Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
	NRFWrite_Buf(WR_TX_PLOAD , TxDate , TX_DATA_WITDH);                 //д������ 
 
	CE=1;
	Delay(5);//����10us������
}

void NRFSetTxMode3(unsigned char *TxDate)				   //���͸��ӻ�3
{
    CE=0;
		 
   	NRFWrite_Buf(WRITE_REG + TX_ADDR , Tx_Addr3 , TX_ADDR_WITDH);       //д�Ĵ���ָ��+���յ�ַʹ��ָ�� �����յ�ַ ����ַ���
	NRFWrite_Buf(WRITE_REG + RX_ADDR_P3 , Tx_Addr3 , TX_ADDR_WITDH);    //Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
	NRFWrite_Buf(WR_TX_PLOAD , TxDate , TX_DATA_WITDH);                 //д������ 
 
	CE=1;
	Delay(5);//����10us������
}
/*
void NRFSetTxMode4(uchar *TxDate)				   //���͸��ӻ�4
{
    CE=0;
		 
   	NRFWrite_Buf(WRITE_REG + TX_ADDR , Rx_Addr4 , TX_ADDR_WITDH);       //д�Ĵ���ָ��+���յ�ַʹ��ָ�� �����յ�ַ ����ַ���
	NRFWrite_Buf(WRITE_REG + RX_ADDR_P4 , Rx_Addr4 , TX_ADDR_WITDH);    //Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
	NRFWrite_Buf(WR_TX_PLOAD , TxDate , TX_DATA_WITDH);                 //д������ 
 
	CE=1;
	Delay(5);//����10us������
}
 
void NRFSetTxMode5(uchar *TxDate)				   //���͸��ӻ�5
{
    CE=0;
		 
   	NRFWrite_Buf(WRITE_REG + TX_ADDR , Rx_Addr5 , TX_ADDR_WITDH);       //д�Ĵ���ָ��+���յ�ַʹ��ָ�� �����յ�ַ ����ַ���
	NRFWrite_Buf(WRITE_REG + RX_ADDR_P5 , Rx_Addr5 , TX_ADDR_WITDH);    //Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
	NRFWrite_Buf(WR_TX_PLOAD , TxDate , TX_DATA_WITDH);                 //д������ 
 
	CE=1;
	Delay(5);//����10us������
}
 
 */
  
/****************************���Ӧ���ź�******************************/

 //���ڷ���ģʽ����Ӧ���ź� 
unsigned char CheckACK()
{ 	
	sta = NRFReadReg(READ_REG+STATUS);                    // ����״̬�Ĵ���
	if(TX_DS||MAX_RT)                                      //��������жϣ����յ�Ӧ���ź� ���� �Զ��ط����������
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
 