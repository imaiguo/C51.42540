#include"reg52.h"
#include"nrf24l01.h"
#include"delay.h"
/***************************��ʱ����****************/
void Delay(unsigned int t)
{
  unsigned int x,y;
  for(x=t;x>0;x--)
   for(y=110;y>0;y--);
}
/**********************************************
   DS18B20ר����ʱ�Ӻ���
**********************************************/
void DDelay(unsigned int t) 
{ 
	unsigned int n;
	n=0; 
	while(n<t) 
	{
	  n++;
	} 
}