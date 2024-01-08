#include<reg51.h>

#define turnoff 0
#define level1 1
#define level2 2
#define level3 3
#define nokey  4

void delayMs(unsigned int xms);
void delayUs();

unsigned char keyscan();

void lcd_init();
void display(unsigned char speed,int v);
void writeComm(unsigned char comm);

int getTmpValue();
void sendChangeCmd();

bit PCF8591_SendByte(unsigned char channel);
unsigned char PCF8591_RcvByte();

void Ircordpro(void);