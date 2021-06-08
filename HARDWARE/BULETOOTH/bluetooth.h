#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H 
#include "sys.h"

#define RTS PAin(5)

void uart3_init(u32 bound);
void MY_SEND_BUFF(char *buff);
void USART3_IRQHandler(void);


#endif

