#include"reg52.h"
#include"nrf24l01.h"
#include"delay.h"
/**********************ÑÓÊ±º¯Êý**************************/
void NRFDelay(unint t)
{
   unint x,y;
   for(x=t;x>0;x--)
    for(y=110;y>0;y--);
}