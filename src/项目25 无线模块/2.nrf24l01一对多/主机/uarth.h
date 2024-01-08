#ifndef __UART_H__
#define __UART_H__

#define F_OSC 12000000
#define F_BAUD 9600

void uart( unsigned int t );
void uart_init();
#endif