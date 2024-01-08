/**********************BST-M51ʵ�鿪��������************************
*  ƽ̨��BST-M51 + Keil U4 + STC89C52
*  ���ƣ�8*8����ģ��ʵ��
*  ��˾���������ǲ�����������޹�˾       
*  ���ڣ�2015-7
*  ����:11.0592MHZ
*  ʵ��Ч������ʾ������״
******************************************************************/
#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char
#define uint  unsigned int
//����Max7219�˿�
sbit Max7219_pinCLK = P2^2;//CLK
sbit Max7219_pinCS  = P2^1;//CS
sbit Max7219_pinDIN = P2^0;//DIN
uchar code disp1[8]=
{0x66,0x99,0x81,0x81,0x42,0x24,0x18,0x00};//����

void Delay_xms(uint x)
{
 uint i,j;
 for(i=0;i<x;i++)
  for(j=0;j<112;j++);
}
//--------------------------------------------
//���ܣ���MAX7219(U3)д���ֽ�
//��ڲ�����DATA 
//���ڲ�������
//˵����
void Write_Max7219_byte(uchar DATA)         
{
    	uchar i;    
		Max7219_pinCS=0;		
	    for(i=8;i>=1;i--)
          {		  
            Max7219_pinCLK=0;
            Max7219_pinDIN=DATA&0x80;
            DATA=DATA<<1;
            Max7219_pinCLK=1;
           }                                 
}
//-------------------------------------------
//���ܣ���MAX7219д������
//��ڲ�����address��dat
//���ڲ�������
//˵����
void Write_Max7219(uchar address,uchar dat)
{ 
     Max7219_pinCS=0;
	 Write_Max7219_byte(address);           //д���ַ��������ܱ��
     Write_Max7219_byte(dat);               //д�����ݣ����������ʾ���� 
	 Max7219_pinCS=1;                        
}

void Init_MAX7219(void)
{
 Write_Max7219(0x09, 0x00);       //���뷽ʽ��BCD��
 Write_Max7219(0x0a, 0x03);       //���� 
 Write_Max7219(0x0b, 0x07);       //ɨ����ޣ�8���������ʾ
 Write_Max7219(0x0c, 0x01);       //����ģʽ��0����ͨģʽ��1
 Write_Max7219(0x0f, 0x00);       //��ʾ���ԣ�1�����Խ�����������ʾ��0
}
void main(void)
{
 uchar i;
 Delay_xms(50);
 Init_MAX7219();  
 for(i=1;i<9;i++)
    Write_Max7219(i,disp1[i-1]); 
}
