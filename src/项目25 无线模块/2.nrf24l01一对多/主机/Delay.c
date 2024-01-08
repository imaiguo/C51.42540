/**********************BST-M51实验开发板例程************************
*  平台：BST-M51 + Keil U4 + STC89C52
*  名称：24L01无线发射（一对多）
*  公司：深圳市亚博软件开发有限公司       
*  日期：2015-6
*  晶振:11.0592MHZ
******************************************************************/
/***************************延时函数****************/
void Delay(unsigned int t)
{
  unsigned char x,y;
  for(x=t;x>0;x--)
   for(y=125;y>0;y--);
}

/**********************************************
   DS18B20专用延时子函数
**********************************************/
void DDelay(unsigned int t) 
{ 
	unsigned char n;
	n=0; 
	while(n<t) 
	{
	  n++;
	} 
}



void Delay_x_ms(unsigned int x)
{
     unsigned char i,j ;
   while(x--)
{
   for(i=22;i>0;i--)
    for(j=10;j>0;j--);
}

}

