#ifndef _LED_H
#define _LED_H

#include "sys.h"

#define LED PCout(13)  //�͵�ƽ��

#define BEEP PBout(3)  //�ߵ�ƽ��

void LED_init(void);
void BEEP_init(void);


#endif

