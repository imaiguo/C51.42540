/**********************BST-V51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：红外接收模块实验
*  公司：深圳市亚博软件开发有限公司      
*  日期：2015-6
*  晶振:11.0592MHZ
*  说明：免费开源，不提供源代码分析.
******************************************************************/
#include<reg51.h>
#include<allhead.h>

sbit IR=P3^2;  //红外接口标志
unsigned char  irtime;//红外用全局变量

bit irpro_ok,irok;
unsigned char IRcord[4];
unsigned char irdata[33];



  
  