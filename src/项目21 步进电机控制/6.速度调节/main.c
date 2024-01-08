/**********************BST-V51ʵ�鿪��������************************
*  ƽ̨��BST-M51 + Keil U4 + STC89C52RD
*  ���ƣ������������
*  ��˾���������ǲ�����������޹�˾      
*  ����:11.0592MHZ
*  ʵ��Ч�������������ڲ���4�ಽ������������� 4���������� �ֱ���Ƽ��١����١�������ֹͣ       
		�������ʾ 01-18�ٶȵȼ�������Խ���ٶ�Խ��
******************************************************************/
#include <reg52.h>

#define dula P0		//��ѡ�źŵ�����������
sbit wei1=P2^4;
sbit wei2=P2^5;
sbit wei3=P2^6;
sbit wei4=P2^7;

unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,
                        0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

sbit A1=P1^0; //���岽��������Ӷ˿�
sbit B1=P1^1;
sbit C1=P1^2;
sbit D1=P1^3;
sbit k1 = P3^4;
sbit k2 = P3^5;
sbit k3 = P3^6;
sbit k4 = P3^7;


#define Coil_CD1 {A1=1;B1=1;C1=0;D1=0;}//CD��ͨ�磬������ϵ�
#define Coil_AD1 {A1=0;B1=1;C1=1;D1=0;}//AD��ͨ�磬������ϵ�
#define Coil_AB1 {A1=0;B1=0;C1=1;D1=1;}//AB��ͨ�磬������ϵ�
#define Coil_BC1 {A1=1;B1=0;C1=0;D1=1;}//BC��ͨ�磬������ϵ�
#define Coil_A1 {A1=0;B1=1;C1=1;D1=1;}//A��ͨ�磬������ϵ�
#define Coil_B1 {A1=1;B1=0;C1=1;D1=1;}//B��ͨ�磬������ϵ�
#define Coil_C1 {A1=1;B1=1;C1=0;D1=1;}//C��ͨ�磬������ϵ�
#define Coil_D1 {A1=1;B1=1;C1=1;D1=0;}//D��ͨ�磬������ϵ�
#define Coil_OFF {A1=1;B1=1;C1=1;D1=1;}//ȫ���ϵ�

unsigned char Speed=1;
bit StopFlag;
void Display(unsigned char num);
void Init_Timer0(void);
/*------------------------------------------------
��ʱ���������뼶��
------------------------------------------------*/
void delay(unsigned int xms)				
{
	unsigned int i,j;
	for(i=xms;i>0;i--)		      //i=xms����ʱԼxms����
		for(j=112;j>0;j--);
}


/*------------------------------------------------
                    ������
------------------------------------------------*/
main()
{
 unsigned int i=512;//��תһ��ʱ��
 //unsigned char num;
 Init_Timer0();
 Coil_OFF
 while(1)  //����
  { 
   if(k1 == 0)//��һ������,�ٶȵȼ�����
	{
		delay(10);//��ʱ10ms ����
		if(k1 == 0)//�ٴ��жϰ����Ƿ񱻰���
		{
			while(k1 == 0);//ֱ�������ж��ɿ�
			if(Speed<18)
				Speed++;	
		}
	}
	else if(k2 == 0)//�ڶ����������ٶȵȼ���С
		{
			delay(10);//��ʱ10ms ����
			if(k2 == 0)//�ٴ��жϰ����Ƿ񱻰���
			{
				while(k2 == 0);//ֱ�������ж��ɿ�
				if(Speed>1)
	   				Speed--;
			}
		}
	else if(k3 == 0)//��������������ͣ
		{
			Coil_OFF
  			StopFlag=1;
		}
	else if(k4 == 0)//���ĸ�����������
		StopFlag=0;
	Display(Speed);
  }
  
}

/*------------------------------------------------
 �������ʾ���������ڶ�̬ɨ�������
------------------------------------------------*/
void Display(unsigned char num)
{
		unsigned char shi,ge;

		shi = num/10;
		ge = num%10;

		wei1=0;		//��ʾʮλ
		wei2=0;
		wei3=1;
		wei4=0;
		P0=table[shi];
		delay(2);
		
		wei1=0;		//��ʾ��λ
		wei2=0;
		wei3=0;
		wei4=1;
		P0=table[ge];
		delay(2);
}
/*------------------------------------------------
                    ��ʱ����ʼ���ӳ���
------------------------------------------------*/
void Init_Timer0(void)
{
 TMOD |= 0x01;	  //ʹ��ģʽ1��16λ��ʱ����ʹ��"|"���ſ�����ʹ�ö����ʱ��ʱ����Ӱ��		     
 //TH0=0x00;	      //������ֵ
 //TL0=0x00;
 EA=1;            //���жϴ�
 ET0=1;           //��ʱ���жϴ�
 TR0=1;           //��ʱ�����ش�
}
/*------------------------------------------------
                 ��ʱ���ж��ӳ���
------------------------------------------------*/
void Timer0_isr(void) interrupt 1 
{
 static unsigned char times,i;
 TH0=(65536-1000)/256;		  //���¸�ֵ 1ms
 TL0=(65536-1000)%256;
 
 if(!StopFlag)
   {
  if(times==(20-Speed))//���ֵ18��������С���ֵ20-18=2
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


