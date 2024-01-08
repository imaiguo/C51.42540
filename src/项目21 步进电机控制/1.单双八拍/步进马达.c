/*************************************************************************************
*  标题:  步进电机试验一															 *																					 *
*    通过本例程了解步进马达使用及驱动程序编写										 *
*;   单双八拍工作方式：                                                           	 *
*;   A-AB-B-BC-C-CD-D-DA      								 *               	                            	 *
                    	                          									 *
**************************************************************************************/  
//接线方法：请你一定要看，步进电机杜邦线接方法 步进电机模块上“-”接到开发板上的GND “+接VCC
//IN1~IN4 分别接到P1.0~P1.3”
 #include "reg52.h"
 //Motor
sbit A = P1^0;	  //定义管脚
sbit b = P1^1;
sbit C = P1^2;
sbit D = P1^3;

 ///////////////////////////////////////
//步进电机驱动
unsigned char MotorStep=0;  //步进马达步序
unsigned int  Speed=1,TIM,CT;

#define speed 12   // 调整速度 数值不要设的太低 低了会引起震动。

/*****************************************************
*	    初始化马达							         *
******************************************************/
void InitMotor()
{
  A = 1;
  b = 1;
  C = 1;
  D = 1;
}


 
void SetMotor()
{
    switch(MotorStep)
    {
       case 0:
          if(TIM)   // A
          {				    
            A = 0;		    //0xf1  
            b = 1;
			C = 1;
            D = 1;
            MotorStep = 1;
			TIM=0;
          }
       break;

       case 1:		 // AB
         if(TIM)
          {	
            A = 0;		   //0xf3 
            b = 0;
           	C = 1;
            D = 1;
            MotorStep = 2;
			TIM=0;
          }
       break;

       case 2:	   //B
         if(TIM)
          {
           
          	A = 1;
            b = 0;		  //0xf2 
            C = 1;
            D = 1;
           
            MotorStep = 3;
			TIM=0;
          }
       break;


       case 3:		//BC
         if(TIM)
          {	
            A = 1;
            b = 0;		    //0xf6
            C = 0;
            D = 1;
            MotorStep = 4;
			TIM=0;
          }
       break;
     case 4:		 //C
         if(TIM)
          {	
            A = 1;
            b = 1;		    //0xf4
            C = 0;
            D = 1;
            MotorStep = 5;
			TIM=0;
           }
       break;
	case 5:			  //CD
         if(TIM)
          {	
            A = 1;
            b = 1;		    //0xfc
            C = 0;
            D = 0;
            MotorStep = 6;
			TIM=0;
          }
       break;
	case 6:			  //D
         if(TIM)
          {	
            A = 1;
            b = 1;		    //0xf8
            C = 1;
            D = 0;
            MotorStep = 7;
			TIM=0;
          }
       break;
	case 7:			//DA
         if(TIM)
          {	
            A = 0;
            b = 1;		    //0xf9
            C = 1;
            D = 0;
            MotorStep = 0;
			TIM=0;
          }
       break;
	
	}
}


void system_Ini()
{
    TMOD|= 0x11;
    TH0=0xDC; //11.0592M,定时10ms
    TL0=0x00;	
	IE = 0x8A;	
    TR0  = 1;
}

main() 
{ 
	system_Ini();
	InitMotor();
	while(1)
	{
	 SetMotor();
	}
}

/************************************************
*												*
*	      定时中断延时						    *
************************************************/
  void Tzd(void) interrupt 1
{
   	TH0 = 0xDC;    //11.0592，定时10ms
	TL0 = 0x00;

	TIM=1;
 }
