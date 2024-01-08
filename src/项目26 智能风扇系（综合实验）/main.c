/**********************BST-M51ʵ�鿪��������************************
*  ƽ̨��BST-M51 + Keil U4 + STC89C52
*  ���ƣ����ܷ���
*  ��˾���������ǲ�����������޹�˾       
*  ���ڣ�2015-8
*  ����:11.0592MHZ
******************************************************************/
#include<reg51.h>
#include<allhead.h>

#define Coil_CD1 {A1=1;B1=1;C1=0;D1=0;}//CD��ͨ�磬������ϵ�
#define Coil_AD1 {A1=0;B1=1;C1=1;D1=0;}//AD��ͨ�磬������ϵ�
#define Coil_AB1 {A1=0;B1=0;C1=1;D1=1;}//AB��ͨ�磬������ϵ�
#define Coil_BC1 {A1=1;B1=0;C1=0;D1=1;}//BC��ͨ�磬������ϵ�
#define Coil_A1 {A1=0;B1=1;C1=1;D1=1;}//A��ͨ�磬������ϵ�
#define Coil_B1 {A1=1;B1=0;C1=1;D1=1;}//B��ͨ�磬������ϵ�
#define Coil_C1 {A1=1;B1=1;C1=0;D1=1;}//C��ͨ�磬������ϵ�
#define Coil_D1 {A1=1;B1=1;C1=1;D1=0;}//D��ͨ�磬������ϵ�
#define Coil_OFF {A1=1;B1=1;C1=1;D1=1;}//ȫ���ϵ�

sbit A1=P1^4; //���岽��������Ӷ˿�
sbit B1=P1^5;
sbit C1=P1^6;
sbit D1=P1^7;
sbit IR=P3^2;  //����ӿڱ�־

unsigned char speed=0,key0=0,adda,key,irtime,irdata[33],IRcord[4];
int tmp;
unsigned int count=0;
bit irpro_ok,irok;
/*------------------------------------------------
                ������ֵ����
------------------------------------------------*/
void Ircordpro(void)//������ֵ������
{ 
  unsigned char i, j, k;
  unsigned char cord,value;

  k=1;
  for(i=0;i<4;i++)      //����4���ֽ�
     {
      for(j=1;j<=8;j++) //����1���ֽ�8λ
         {
          cord=irdata[k];
          if(cord>7)//����ĳֵΪ1������;����о��Թ�ϵ������ʹ��12M���㣬��ֵ������һ�����
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
	 irpro_ok=1;//������ϱ�־λ��1
}
/*------------------------------------------------
                  ��ֵ����
------------------------------------------------*/

unsigned char Ir_work(void)//�����ֵɢת����
{
	unsigned char key;
	switch(IRcord[2])//�жϵ���������ֵ
	{
		 case 0x0c:key=level1;break;//1 ��ʾ��Ӧ�İ���ֵ
		 case 0x18:key=level2;break;//2
		 case 0x5e:key=level3;break;//3
	     default:break;
	}	
	irpro_ok=0;//������ɱ�־
	return key;
}
/*------------------------------------------------
                    ��ʱ��0��ʼ���ӳ���
------------------------------------------------*/
void timer0_init(void)
{
 TMOD |= 0x01;	  //��ʱ��0������ʽ2��TH0����װֵ��TL0�ǳ�ֵ��ʹ��"|"���ſ�����ʹ�ö����ʱ��ʱ����Ӱ��		     
 TH0=0xFC;	      //������ֵ1ms
 TL0=0x66;
 EA=1;            //���жϴ�
 ET0=1;           //��ʱ���жϴ�
 TR0=1;           //��ʱ�����ش�
}
/*------------------------------------------------
                    ��ʱ��1��ʼ���ӳ���
------------------------------------------------*/
void timer1_init(void)
{
 TMOD |= 0x01;	  //ʹ��ģʽ1��16λ��ʱ����ʹ��"|"���ſ�����ʹ�ö����ʱ��ʱ����Ӱ��		     
 TH1=0xFC;	      //������ֵ1ms
 TL1=0x66;
 EA=1;            //���жϴ�
 ET1=1;           //��ʱ���жϴ�
 TR1=1;           //��ʱ�����ش�
}
/*------------------------------------------------
                  �ⲿ�ж�0��ʼ��
------------------------------------------------*/
void ex0init(void)
{
 IT0 = 1;   //ָ���ⲿ�ж�0�½��ش�����INT0 (P3.2)
 EX0 = 1;   //ʹ���ⲿ�ж�
 EA = 1;    //�����ж�
}
/*------------------------------------------------
                 ������
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
                 ��ʱ��1�ж��ӳ���
------------------------------------------------*/
void Timer1_isr(void) interrupt 3 
{
 static unsigned char times,i;
 unsigned char t;
 TH1=0xFC;		  //���¸�ֵ 1ms
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
                 ��ʱ��0�ж��ӳ���
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 
{
 static unsigned char tt;
 TH1=0xFC;		  //���¸�ֵ 1ms
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
                  �ⲿ�ж�0�жϴ���
------------------------------------------------*/
void EX0_ISR (void) interrupt 0 //�ⲿ�ж�0������
{
	static unsigned char  i;             //���պ����źŴ���
	static bit startflag;                //�Ƿ�ʼ�����־λ

	if(startflag)                         
	{
		if(irtime<17&&irtime>=9)//������ TC9012��ͷ�룬9ms+4.5ms
	    	i=0;
		irdata[i]=irtime;//�洢ÿ����ƽ�ĳ���ʱ�䣬�����Ժ��ж���0����1
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