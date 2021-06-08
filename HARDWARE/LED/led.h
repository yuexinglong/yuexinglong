#ifndef _LED_H
#define _LED_H

#include "sys.h"

#define LED PCout(13)  //低电平亮

#define BEEP PBout(3)  //高电平响

void LED_init(void);
void BEEP_init(void);


#endif

