/**********************BST-M51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：按键扫描
*  公司：深圳市亚博软件开发有限公司       
*  日期：2015-8
*  晶振:11.0592MHZ
******************************************************************/
#include<reg51.h>
#include<allhead.h>
#include <math.H>

sbit rw = P1^1; 
sbit RS = P1^0; 
sbit LCDEN = P2^5;

void writeComm(unsigned char comm)
{
    RS = 0;    
    P0 = comm;
    LCDEN = 1;
    delayUs();
    LCDEN = 0;
    delayMs(1);
}

void writeData(unsigned char dat)
{
    RS = 1;
    P0 = dat;
    LCDEN = 1;
    delayUs();
    LCDEN = 0;
    delayMs(1);
}

void writeString(unsigned char * str,unsigned char length)
{
    unsigned char i;
    for(i = 0; i < length; i++)
    {
         writeData(str[i]);
     }
}

void lcd_init()
{
    rw = 0; 
    writeComm(0x38);
    writeComm(0x0c); 
    writeComm(0x06);
    writeComm(0x01); 
}

void display(unsigned char speed,int v) 
{
    unsigned char datas[9];
	unsigned char level[7];

	level[0]='l';
	level[1]='e';
	level[2]='v';
	level[3]='e';
	level[4]='l';
	level[5]=' ';
	level[6]='0'+speed;
	writeComm(0x80+4);
    writeString(level, 7);

	datas[0] ='T';
	datas[1] ='E';
	datas[2] ='M';
	datas[3] ='P';
	datas[4] =':';
    datas[5] = '0'+(v/100)%10;
    datas[6] = '0'+(v/10)%10;
    datas[7] = '.';
    datas[8] = '0'+v%10;
    writeComm(0xc0+3);
    writeString(datas, 9);
}