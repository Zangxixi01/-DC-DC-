#ifndef __SD_H
#define __SD_H
#include "main.h"
void SD_Init(void);
#define SD_ON  GPIO_SetBits(GPIOA,GPIO_Pin_9)
#define SD_OFF GPIO_ResetBits(GPIOA,GPIO_Pin_9)
#endif


