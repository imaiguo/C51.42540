/**********************BST-M51ʵ�鿪��������************************
*  ƽ̨��BST-M51 + Keil U4 + STC89C52
*  ���ƣ�18b20�¶���ʾ������ܣ�ʵ��
*  ��˾���������ǲ�����������޹�˾       
*  ���ڣ�2015-7
*  ����:11.0592MHZ
*  ʵ��Ч��������18B20�¶ȴ������������ʾ����ǰ�¶�
******************************************************************/
#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
#define dula P0		//��ѡ�źŵ�����������
sbit wei1=P2^4;
sbit wei2=P2^5;
sbit wei3=P2^6;
sbit wei4=P2^7;
sbit DS=P2^2;           //define interface
uint temp;             // variable of temperature

unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
unsigned char code table1[]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,
                        0x87,0xff,0xef};

void delay(uint count)      //delay
{
  uint i;
  while(count)
  {
    i=200;
    while(i>0)
    i--;
    count--;
  }
}


void dsreset(void)       //send reset and initialization command
{
  uint i;
  DS=0;		              
  i=103;				   //����������480us~960us
  while(i>0)i--;
  DS=1;					   //Ȼ���������ߣ���DS18B20������Ӧ�Ὣ��15us~60us����������
  i=4;					   //15us~60us�ȴ�
  while(i>0)i--;
  //while(DS);
}

bit tmpreadbit(void)       //read a bit
{
   uint i;
   bit dat;
   DS=0;i++;          //i++ for delay
   DS=1;i++;i++;
   dat=DS;
   i=8;while(i>0)i--;
   return (dat);
}

uchar tmpread(void)   //read a byte date
{
  uchar i,j,dat;
  dat=0;
  for(i=1;i<=8;i++)
  {
    j=tmpreadbit();
    dat=(j<<7)|(dat>>1);   //�������������λ����ǰ�棬�����պ�һ���ֽ���DAT��
  }
  return(dat);
}

void tmpwritebyte(uchar dat)   //write a byte to ds18b20
{
  uint i;
  uchar j;
  bit testb;
  for(j=1;j<=8;j++)
  {
    testb=dat&0x01;
    dat=dat>>1;
    if(testb)     //write 1
    {
      DS=0;
      i++;i++;
      DS=1;
      i=8;while(i>0)i--;
    }
    else
    {
      DS=0;       //write 0
      i=8;while(i>0)i--;
      DS=1;
      i++;i++;
    }

  }
}

void tmpchange(void)  //DS18B20 begin change
{
  dsreset();
  delay(1);
  tmpwritebyte(0xcc);  // address all drivers on bus
  tmpwritebyte(0x44);  //  initiates a single temperature conversion
  //delay(100);
}

uint tmp()               //get the temperature
{
  float tt;
  uchar a,b;
  dsreset();
  delay(1);
  tmpwritebyte(0xcc);
  tmpwritebyte(0xbe);
  a=tmpread();//�Ͱ�λ
  b=tmpread();//�߰�λ
  temp=b;
  temp<<=8;             //two byte  compose a int variable
  temp=temp|a;
  tt=temp*0.0625; //��������ǲ⵽���¶ȣ���ֵ�ɵ�С�������λ
  temp=tt*10+0.5; //Ϊ����ʾ�¶Ⱥ��С�����һλ�������������룬��Ϊȡֵ���㲻��ȡС��������
  return temp;
}

void display(uint temp)			//��ʾ����
{
   uchar bai,shi1,shi0,ge;
   bai=temp/100;//�¶���ֵ��Ϊʮλ
   shi0=temp%100;//�¶���ֵ��Ϊ���㼸
   shi1=shi0/10;//�¶���Ϊ��λ��������ʾʱ��Ҫ��С����
   ge=shi0%10;//�¶���ΪС��λ�����Ѿ���������

   wei1=1;		//��ʾ��λ
   wei2=0;
   wei3=0;
   wei4=0;
   P0=table[bai];
   delay(2);

   wei1=0;		//��ʾʮλ
   wei2=1;
   wei3=0;
   wei4=0;
   P0=table1[shi1];
   delay(2);

   wei1=0;		//��ʾ��λ
   wei2=0;
   wei3=1;
   wei4=0;
   P0=table[ge];
   delay(2);
}


void main()
{
 uchar a;
  do
  {
    tmpchange();//��18b20��ʼת���¶�

	for(a=100;a>0;a--)
  	{   
		display(tmp());
  	}
  }while(1);
}

