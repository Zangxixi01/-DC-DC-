#ifndef __SW_H
#define __SW_H
#include "main.h"
void SW_Init(void);
#define DCIN_ON  GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define DCIN_OFF GPIO_ResetBits(GPIOA,GPIO_Pin_6)
#define DCOUT_ON  GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define DCOUT_OFF GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#endif


