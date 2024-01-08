/**********************BST-V51ʵ�鿪��������************************
*  ƽ̨��BST-M51 + Keil U4 + STC89C52
*  ���ƣ�ADģ��ʵ��
*  ��˾���������ǲ�����������޹�˾       
*  ���ڣ�2015-7
*  ����:11.0592MHZ
*  ʵ��Ч������1602Һ��������ʾ��ͨ���ĵ�ѹֵ
******************************************************************/
#include<reg52.h>    //������Ƭ���Ĵ�����ͷ�ļ�
#include <intrins.h> 

#define  AddWr 0x90    //PCF8591 ��ַ

// ��������
unsigned char AD_CHANNEL;
sbit scl=P2^0;       //I2C  ʱ�� 
sbit sda=P2^1;       //I2C  ���� 
bit ack;                 /*Ӧ���־λ*/

sbit RS = P1^0;//Pin4
sbit RW = P1^1; //Pin5
sbit E  = P2^5;//Pin6


#define Data  P0    //���ݶ˿�

unsigned char TempData[8];
unsigned char  SecondLine[]="                ";
unsigned char  FirstLine[] ="                ";
/*******************************************************************
                     �����ߺ���               
����ԭ��: void  Start_I2c();  
����:     ����I2C����,������I2C��ʼ����.  
********************************************************************/
void Start_I2c()
{
  sda=1;         /*������ʼ�����������ź�*/
  _nop_();
  scl=1;
  _nop_();        /*��ʼ��������ʱ�����4.7us,��ʱ*/
  _nop_();
  _nop_();
  _nop_();
  _nop_();    
  sda=0;         /*������ʼ�ź�*/
  _nop_();        /* ��ʼ��������ʱ�����4��s*/
  _nop_();
  _nop_();
  _nop_();
  _nop_();       
  scl=0;       /*ǯסI2C���ߣ�׼�����ͻ�������� */
  _nop_();
  _nop_();
}

/*******************************************************************
                      �������ߺ���               
����ԭ��: void  Stop_I2c();  
����:     ����I2C����,������I2C��������.  
********************************************************************/
void Stop_I2c()
{
  sda=0;      /*���ͽ��������������ź�*/
  _nop_();       /*���ͽ���������ʱ���ź�*/
  scl=1;      /*������������ʱ�����4��s*/
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  _nop_();
  sda=1;      /*����I2C���߽����ź�*/
  _nop_();
  _nop_();
  _nop_();
  _nop_();
}

/*******************************************************************
                 �ֽ����ݷ��ͺ���               
����ԭ��: void  I2C_SendByte(UCHAR c);
����:     ������c���ͳ�ȥ,�����ǵ�ַ,Ҳ����������,�����ȴ�Ӧ��,����
          ��״̬λ���в���.(��Ӧ����Ӧ��ʹack=0)     
           ��������������ack=1; ack=0��ʾ��������Ӧ����𻵡�
********************************************************************/
void  I2C_SendByte(unsigned char  c)
{
 unsigned char  i;
 
 for(i=0;i<8;i++)  /*Ҫ���͵����ݳ���Ϊ8λ*/
    {
     if((c<<i)&0x80)sda=1;   /*�жϷ���λ*/
       else  sda=0;                
     _nop_();
     scl=1;               /*��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ*/
      _nop_(); 
      _nop_();             /*��֤ʱ�Ӹߵ�ƽ���ڴ���4��s*/
      _nop_();
      _nop_();
      _nop_();         
     scl=0; 
    }
    
    _nop_();
    _nop_();
    sda=1;                /*8λ��������ͷ������ߣ�׼������Ӧ��λ*/
    _nop_();
    _nop_();   
    scl=1;
    _nop_();
    _nop_();
    _nop_();
    if(sda==1)ack=0;     
       else ack=1;        /*�ж��Ƿ���յ�Ӧ���ź�*/
    scl=0;
    _nop_();
    _nop_();
}

/*******************************************************************
                 �ֽ����ݽ��պ���               
����ԭ��: UCHAR  I2C_RcvByte();
����:        �������մ���������������,���ж����ߴ���(����Ӧ���ź�)��
          ���������Ӧ����Ӧ��ӻ���  
********************************************************************/    
unsigned char   I2C_RcvByte()
{
  unsigned char  retc=0,i; 
  sda=1;                     /*��������Ϊ���뷽ʽ*/
  for(i=0;i<8;i++)
      {
        _nop_();           
        scl=0;                  /*��ʱ����Ϊ�ͣ�׼����������λ*/
        _nop_();
        _nop_();                 /*ʱ�ӵ͵�ƽ���ڴ���4.7��s*/
        _nop_();
        _nop_();
        _nop_();
        scl=1;                  /*��ʱ����Ϊ��ʹ��������������Ч*/
        _nop_();
        _nop_();
        retc=retc<<1;
        if(sda==1)retc=retc+1;  /*������λ,���յ�����λ����retc�� */
        _nop_();
        _nop_(); 
      }
  scl=0;    
  _nop_();
  _nop_();
  return(retc);
}

/********************************************************************
                     Ӧ���Ӻ���
����ԭ��:  void Ack_I2c(bit a);
����:      ����������Ӧ���ź�(������Ӧ����Ӧ���źţ���λ����a����)
********************************************************************/
void Ack_I2c(bit a)
{  
  if(a==0)sda=0;              /*�ڴ˷���Ӧ����Ӧ���ź� */
  else sda=1;				  /*0Ϊ����Ӧ��1Ϊ��Ӧ���ź� */
  _nop_();
  _nop_();
  _nop_();      
  scl=1;
  _nop_();
  _nop_();                    /*ʱ�ӵ͵�ƽ���ڴ���4��s*/
  _nop_();
  _nop_();
  _nop_();  
  scl=0;                     /*��ʱ���ߣ�סI2C�����Ա��������*/
  _nop_();
  _nop_();    
}

/************************************************************
* ������        : Pcf8591_DaConversion
* ��������      : PCF8591����������ģ����
* ����          : addr��������ַ����channel��ת��ͨ������value��ת������ֵ��
* ���         	: ��
******************* *****************************************/
bit Pcf8591_DaConversion(unsigned char addr,unsigned char channel,  unsigned char Val)
{
   Start_I2c();              //��������
   I2C_SendByte(addr);            //����������ַ
   if(ack==0)return(0);
   I2C_SendByte(0x40|channel);              //���Ϳ����ֽ�
   if(ack==0)return(0);
   I2C_SendByte(Val);            //����DAC����ֵ  
   if(ack==0)return(0);
   Stop_I2c();               //��������
   return(1);
}

/************************************************************
* ������        : Pcf8591_SendByte
* ��������		: д��һ����������
* ����          : addr��������ַ����channel��ת��ͨ����
* ���         	: ��
************************************************************/
bit PCF8591_SendByte(unsigned char addr,unsigned char channel)
{
   Start_I2c();              //��������
   I2C_SendByte(addr);            //����������ַ
   if(ack==0)return(0);
   I2C_SendByte(0x40|channel);              //���Ϳ����ֽ�
   if(ack==0)return(0);
   Stop_I2c();               //��������
   return(1);
}

/************************************************************
* ������       	: PCF8591_RcvByte
* ��������   	: ��ȡһ��ת��ֵ
* ����          :
* ���          : dat
************************************************************/
unsigned char PCF8591_RcvByte(unsigned char addr)
{  
   unsigned char dat;

   Start_I2c();          //��������
   I2C_SendByte(addr+1);      //����������ַ
   if(ack==0)return(0);
   dat=I2C_RcvByte();          //��ȡ����0

   Ack_I2c(1);           //���ͷ�Ӧ���ź�
   Stop_I2c();           //��������
   return(dat);
}

/******************************************************************/
/*                   1602Һ������غ���                                     */
/******************************************************************/
void DelayUs(unsigned char us)//delay us
{
 unsigned char uscnt;
 uscnt=us>>1;/* Crystal frequency in 12MHz*/
 while(--uscnt);
}
/******************************************************************/
void DelayMs(unsigned char ms)//delay Ms
{
 while(--ms)
   {
     DelayUs(250);
     DelayUs(250);
	 DelayUs(250);
	 DelayUs(250);
   }
}
void WriteCommand(unsigned char c)
{
 DelayMs(5);//short delay before operation
 E=0;
 RS=0;
 RW=0;
 _nop_();
 E=1;
 Data=c;
 E=0;
}
/****************************************************************/
void WriteData(unsigned char c)
{
 DelayMs(5);   //short delay before operation
 E=0;
 RS=1;
 RW=0;
 _nop_();
 E=1;
 Data=c;
 E=0;
 RS=0;
}
/*********************************************************************/
void ShowChar(unsigned char pos,unsigned char c)
{
 unsigned char p;
 if (pos>=0x10)
    p=pos+0xb0; //�ǵڶ�������������4λΪ0xc
 else 
    p=pos+0x80; //�ǵڶ�������������4λΪ0x8
 WriteCommand (p);//write command
 WriteData (c);   //write data
}
/*************************************************************************/
void ShowString (unsigned char line,char *ptr)
{
 unsigned char l,i;
 l=line<<4;
 for (i=0;i<16;i++)
  ShowChar (l++,*(ptr+i));//ѭ����ʾ16���ַ�
}
/*********************************************************************/
void InitLcd()
{
 DelayMs(15);
 WriteCommand(0x38); //display mode
 WriteCommand(0x38); //display mode
 WriteCommand(0x38); //display mode
 WriteCommand(0x06); //��ʾ����ƶ�λ��
 WriteCommand(0x0c); //��ʾ�����������
 WriteCommand(0x01); //��ʾ����

}


/////////////////////////////////////////////////////////////////
void disp(void)

{


 FirstLine[2]='0'+TempData[0];
 FirstLine[4]='0'+TempData[1];
 FirstLine[3]='.';
 FirstLine[6]='V';

 FirstLine[9]='0'+TempData[2];
 FirstLine[11]='0'+TempData[3];
 FirstLine[10]='.';
 FirstLine[13]='V';



 
 SecondLine[2]='0'+TempData[4];
 SecondLine[4]='0'+TempData[5];
 SecondLine[3]='.';
 SecondLine[6]='V';

 SecondLine[9]='0'+TempData[6];
 SecondLine[11]='0'+TempData[7];
 SecondLine[10]='.';
 SecondLine[13]='V';

ShowString(0,FirstLine);

ShowString(1,SecondLine);

}
/*------------------------------------------------
                    ��ʱ����
------------------------------------------------*/
 void mDelay(unsigned char j)
 {
  unsigned int i;
  for(;j>0;j--)
     {
	  for(i=0;i<125;i++)
	     {;}
	  }
  }
/*------------------------------------------------
                    ������
------------------------------------------------*/
main()
{  
	 unsigned char ADtemp;                //�����м����
	 InitLcd();
     mDelay(20);

	while(1)
	{

	/********����AD-DA����*************/  
	
	    switch(AD_CHANNEL)
		{
			case 0: PCF8591_SendByte(AddWr,1);
			     ADtemp = PCF8591_RcvByte(AddWr);  //ADC0 ģ��ת��1      ��������
				 TempData[0]=ADtemp/50;//����0ͨ����ѹ��ʾ
  				 TempData[1]=(ADtemp%50)/10;
				 break;  
			
			case 1: PCF8591_SendByte(AddWr,2);
			     ADtemp=PCF8591_RcvByte(AddWr);  //ADC1  ģ��ת��2	  ��������
				 TempData[2]=ADtemp/50;//����1ͨ����ѹ��ʾ
  				 TempData[3]=(ADtemp%50)/10;
				 break;  
			
			case 2: PCF8591_SendByte(AddWr,3);
			     ADtemp=PCF8591_RcvByte(AddWr);  //ADC2	ģ��ת��3	   ����
				 TempData[4]=ADtemp/50;//����2ͨ����ѹ��ʾ
  				 TempData[5]=(ADtemp%50)/10;
				 break;  
			
			case 3: PCF8591_SendByte(AddWr,0);
			     ADtemp=PCF8591_RcvByte(AddWr);  //ADC3   ģ��ת��4	   �ɵ�0-5v
				 TempData[6]=ADtemp/50;//����3ͨ����ѹ��ʾ
  				 TempData[7]=(ADtemp%50)/10;
				 break;  
			
			case 4: Pcf8591_DaConversion(AddWr,0, ADtemp); //DAC	  ��ģת��
			     break;
		}			
	   if(++AD_CHANNEL>4) AD_CHANNEL=0;
	   disp();	 
   }
}