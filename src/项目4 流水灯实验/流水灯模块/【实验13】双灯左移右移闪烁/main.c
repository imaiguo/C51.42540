/**********************BST-M51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：流水灯模块实验
*  公司：深圳市亚博软件开发有限公司    
*  日期：2015-6
*  晶振:11.0592MHZ
*  说明：免费开源，不提供源代码分析.
******************************************************************/
#include<reg52.h> //包含头文件，一般情况不需要改动，
                  //头文件包含特殊功能寄存器的定义

void Delay(unsigned int t); //函数声明

/*------------------------------------------------
                    主函数
------------------------------------------------*/
void main (void)
{
                  
unsigned char i;  //定义一个无符号字符型局部变量 i 取值范围 0~255
unsigned char temp;//定义临时变量用于转换

	P1=0xFF;//P1口置1
	
while (1)          //主循环
  {
 temp=0xfc;
 P1=temp;
 for(i=0;i<7;i++)   //加入 for循环，表明for循环大括号
                    //中的程序循环执行7次
  {
  Delay(50000);      //调用延时程序；更改延时数字可
                     //以更改延时长度；用于改变闪烁频率
  temp<<=1;
  temp=temp|0x01;    //也可以直接把temp换成P1
  P1=temp;           //之所以加入temp做中间变量，转换
                     //完成后直接赋值到端口，防止直接
                     //操作端口造成的短暂闪烁
  }
 
 temp=0x3f;
 P1=temp;
 for(i=0;i<7;i++)   //加入 for循环，表明for循环大括号
                    //中的程序循环执行7次
  {
  Delay(50000);      //调用延时程序；更改延时数字可以
                     //更改延时长度；用于改变闪烁频率
  temp>>=1;
  temp|=0x80;        //等效于 temp=temp|0x80
  P1=temp;
  }

 for(i=0;i<3;i++)   //加入 for循环，表明for循环大括号
                    //中的程序循环执行3次
  {
  P1=0xff;
  Delay(50000);
  P1=0x00;
  Delay(50000);
  }                //主循环中添加其他需要一直工作的程序
 }
}
/*------------------------------------------------
 延时函数，含有输入参数 unsigned int t，无返回值
 unsigned int 是定义无符号整形变量，其值的范围是
 0~65535
------------------------------------------------*/
void Delay(unsigned int t)
{
 while(--t);
}